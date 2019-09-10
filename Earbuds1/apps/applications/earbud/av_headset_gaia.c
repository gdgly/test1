/*!
\copyright  Copyright (c) 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       
\brief      Handling of the GAIA transport interface

This is a minimal implementation that only supports upgrade.
*/

#ifdef INCLUDE_DFU

#include "av_headset.h"
#include "av_headset_log.h"

#include <panic.h>
#ifdef GAIA_TEST
#include "tws/av_headset_gaia_starot.h"
#endif

/*! Enumerated type of internal message IDs used by this module */
typedef enum av_headset_gaia_internal_messages
{
        /*! Disconnect GAIA */
    APP_GAIA_INTERNAL_DISCONNECT = INTERNAL_MESSAGE_BASE,
};


static void appGaiaMessageHandler(Task task, MessageId id, Message message);
static void appGaiaHandleCommand(Task task, const GAIA_UNHANDLED_COMMAND_IND_T *command);
static bool appGaiaHandleStatusCommand(Task task, const GAIA_UNHANDLED_COMMAND_IND_T *command);
void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                          uint16 payload_length, uint8 *payload);
#ifdef GAIA_TEST
bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                          uint16 payload_length, uint8 *payload);
#else
void appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                          uint16 payload_length, uint8 *payload);
#endif


/********************  PUBLIC FUNCTIONS  **************************/



/*! Initialise the GAIA Module */
void appGaiaInit(void)
{
    gaiaTaskData *this = appGetGaia();

    this->gaia_task.handler = appGaiaMessageHandler;
    this->client_list = appTaskListInit();
    this->connections_allowed = TRUE;

    appGaiaClientRegister(appGetSmTask());

    GaiaInit(appGetGaiaTask(), 1);
}


void appGaiaClientRegister(Task task)
{
    gaiaTaskData* app_gaia = appGetGaia();
    appTaskListAddTask(app_gaia->client_list, task);
}


static void appGaiaSendInitCfm(void)
{
    MessageSend(appGetAppTask(), APP_GAIA_INIT_CFM, NULL);
}


static void appGaiaNotifyGaiaConnected(void)
{
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_CONNECTED);
}


static void appGaiaNotifyGaiaDisconnected(void)
{
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_DISCONNECTED);
}


static void appGaiaNotifyUpgradeActivity(void)
{
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_UPGRADE_ACTIVITY);
}


static void appGaiaNotifyUpgradeConnection(void)
{
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_UPGRADE_CONNECTED);
}


static void appGaiaNotifyUpgradeDisconnection(void)
{
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_UPGRADE_DISCONNECTED);
}


static void appGaiaHandleInitConfirm(const GAIA_INIT_CFM_T *init_cfm)
{

    DEBUG_LOG("appGaiaHandleInitConfirm GAIA_INIT_CFM: %d (succ)",init_cfm->success);

    PanicFalse(init_cfm->success);

    GaiaSetApiMinorVersion(GAIA_API_MINOR_VERSION);
    GaiaSetAppWillHandleCommand(GAIA_COMMAND_DEVICE_RESET, TRUE);

    /* Start the GAIA transports.
       GATT transport doesn't actually need starting, but not harmful. If GAIA 
       changes in future, may help identify a problem sooner */
    GaiaStartService(gaia_transport_spp);
    GaiaStartService(gaia_transport_gatt);

    /* Successful initialisation of the library. The application needs 
     * this to unblock.
     */
    appGaiaSendInitCfm();
}


/*  Accept the GAIA connection if they are allowed, and inform any clients.
 */
static void appGaiaHandleConnectInd(const GAIA_CONNECT_IND_T *ind)
{
    GAIA_TRANSPORT *transport;

    if (!ind || !ind->success)
    {
        DEBUG_LOG("appGaiaHandleConnectInd Success = FAILED");
        return;
    }

    transport = ind->transport;

    if (!transport)
    {
        DEBUG_LOG("appGaiaHandleConnectInd No transport");

        /* Can't disconnect nothing so just return */
        return;
    }

    if (!appGetGaia()->connections_allowed)
    {
        DEBUG_LOG("appGaiaHandleConnectInd GAIA not allowed");
        GaiaDisconnectRequest(transport);
        return;
    }

    DEBUG_LOG("appGaiaHandleConnectInd Success. Transport:%p",transport);

    appSetGaiaTransport(transport);

    GaiaSetSessionEnable(transport, TRUE);
    GaiaOnTransportConnect(transport);

    appGaiaNotifyGaiaConnected();
}


static void appGaiaHandleDisconnectInd(const GAIA_DISCONNECT_IND_T *ind)
{
    DEBUG_LOG("appGaiaHandleDisconnectInd. Transport %p",ind->transport);

        /* GAIA can send IND with a NULL transport. Seemingly after we 
           requested a disconnect (?) */
    if (ind->transport)
    {
        GaiaDisconnectResponse(ind->transport);
        appSetGaiaTransport(NULL);
    }
    appGaiaNotifyGaiaDisconnected();
}


static void appGaiaHandleInternalDisconnect(void)
{
    GAIA_TRANSPORT *transport = appGetGaiaTransport();

    if (transport)
    {
        GaiaDisconnectRequest(transport);
        appSetGaiaTransport(NULL);
    }
}


static void appGaiaMessageHandler(Task task, MessageId id, Message message)
{
    UNUSED(task);

    DEBUG_LOG("appGaiaMessageHandler 0x%X (%d)",id,id);

    switch (id)
    {
        case APP_GAIA_INTERNAL_DISCONNECT:
            appGaiaHandleInternalDisconnect();
            break;

        case GAIA_INIT_CFM:
            appGaiaHandleInitConfirm((const GAIA_INIT_CFM_T*)message);
            break;

        case GAIA_CONNECT_IND:                   /* Indication of an inbound connection */
            appGaiaHandleConnectInd((const GAIA_CONNECT_IND_T *)message);
            break;

        case GAIA_DISCONNECT_IND:                /* Indication that the connection has closed */
            appGaiaHandleDisconnectInd((const GAIA_DISCONNECT_IND_T *)message);
            break;
            
        case GAIA_DISCONNECT_CFM:                /* Confirmation that a requested disconnection has completed */
            /* We probably want to take note of this to send an event to the state
               machine, but it is mainly upgrade we care about. Not gaia connections. */
            DEBUG_LOG("appGaiaMessageHandler GAIA_DISCONNECT_CFM");
            appSetGaiaTransport(NULL);
            break;

        case GAIA_START_SERVICE_CFM:             /* Confirmation that a Gaia server has started */
            DEBUG_LOG("appGaiaMessageHandler GAIA_START_SERVICE_CFM (nothing to do)");
            break;

        case GAIA_DEBUG_MESSAGE_IND:             /* Sent as a result of a GAIA_COMMAND_SEND_DEBUG_MESSAGE command */
            DEBUG_LOG("appGaiaMessageHandler GAIA_DEBUG_MESSAGE_IND");
            break;

        case GAIA_UNHANDLED_COMMAND_IND:         /* Indication that an unhandled command has been received */
            appGaiaHandleCommand(task, (const GAIA_UNHANDLED_COMMAND_IND_T *) message);
            break;

    case GAIA_SEND_PACKET_CFM:               /* Confirmation that a GaiaSendPacket request has completed */
        {
    const GAIA_SEND_PACKET_CFM_T *m = (const GAIA_SEND_PACKET_CFM_T *) message;
#ifdef GAIA_TEST
        uint8 *packet = m->packet;
        uint16 vendor_id = W16(packet + GAIA_OFFS_VENDOR_ID);
        uint16 command_id = W16(packet + GAIA_OFFS_COMMAND_ID);

        if (FALSE == m->success) {
            DEBUG_LOG("appGaiaMessageHandler GAIA_SEND_PACKET_CFM, status is :%d", m->success);
            // todo hjs 发送包成功之后，重新检查是否有新数据需要发送
            if (command_id == GAIA_COMMAND_STAROT_CALL_AUDIO_IND && vendor_id == GAIA_VENDOR_STAROT) {
                appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
            } else if (GAIA_COMMAND_STAROT_CALL_END == command_id && vendor_id == GAIA_VENDOR_STAROT) {
                appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);
            }
        } else {
            if (command_id == GAIA_COMMAND_STAROT_CALL_AUDIO_IND && vendor_id == GAIA_VENDOR_STAROT) {
//                    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
                appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
                starotNotifyAudioForward(TRUE, packet[GAIA_OFFS_PAYLOAD]);
            } else if (command_id == GAIA_COMMAND_STAROT_CALL_END) {
                printf("end send to app success\n");
            }
        }
#endif
        free(m->packet);
    }
        break;


        case GAIA_DFU_CFM:                       /* Confirmation of a Device Firmware Upgrade request */
            /* If the confirm is a fail, then we can raise an error, but
               not much to do */
            DEBUG_LOG("appGaiaMessageHandler GAIA_DFU_CFM");
            break;

        case GAIA_DFU_IND:                       /* Indication that a Device Firmware Upgrade has begun */
            /* This could be used to update the link policy for faster
               data transfer. */
            DEBUG_LOG("appGaiaMessageHandler GAIA_DFU_IND");
            appGaiaNotifyUpgradeActivity();
            break;

        case GAIA_UPGRADE_CONNECT_IND:           /* Indication of VM Upgrade successful connection */
            /* This tells us the type of transport connection made, so we can
               remember it if needed */
            DEBUG_LOG("appGaiaMessageHandler GAIA_UPGRADE_CONNECT_IND");
            appGaiaNotifyUpgradeConnection();
            break;

        case GAIA_UPGRADE_DISCONNECT_IND:
            DEBUG_LOG("appGaiaMessageHandler GAIA_UPGRADE_DISCONNECT_IND");
            appGaiaNotifyUpgradeDisconnection();
            break;

        case GAIA_CONNECT_CFM:                   /* Confirmation of an outbound connection request */
            DEBUG_LOG("appGaiaMessageHandler GAIA_CONNECT_CFM Unexpected");
            break;

        case GAIA_VA_START_CFM:
        case GAIA_VA_DATA_REQUEST_IND:
        case GAIA_VA_VOICE_END_CFM:
        case GAIA_VA_VOICE_END_IND:
        case GAIA_VA_ANSWER_START_IND:
        case GAIA_VA_ANSWER_END_IND:
        case GAIA_VA_CANCEL_CFM:
        case GAIA_VA_CANCEL_IND:
            DEBUG_LOG("appGaiaMessageHandler GAIA_VA_... Unexpected");
            break;

#ifdef GAIA_TEST
        case GAIA_STAROT_COMMAND_IND:
            starotGaiaHandleCommand((GAIA_STAROT_IND_T *) message);
            break;

        case GAIA_STAROT_MORE_SPACE: {
            starotGaiaParseMessageMoreSpace();
        }
            break;
        case GAIA_STAROT_AUDIO_INTERVAL: {
            if (appGetGaia()->needCycleSendAudio > 0) {
                //send, reset timer
                static uint8 data[201];
                for (int i = 1; i <= 2; ++i) {
                    data[0] = i;
                    appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_TRANS_AUDIO, 0xfe, 201, data);
                }
                MessageSendLater(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 1);
            }
        }
            break;
#endif

        default:
            DEBUG_LOG("appGaiaMessageHandler Unknown GAIA message 0x%x (%d)",id,id);
            break;
    }
}

/*! Handle a GAIA_UNHANDLED_COMMAND_IND from the Gaia library */
static void appGaiaHandleCommand(Task task, const GAIA_UNHANDLED_COMMAND_IND_T *command)
{
    bool handled = FALSE;

    DEBUG_LOG("appGaiaHandleCommand GAIA Vendor ID %d , Id:0x%04x Len:%d",
                command->vendor_id, command->command_id, command->size_payload);

    if (command->vendor_id == GAIA_VENDOR_QTIL)
    {
        appGaiaNotifyUpgradeActivity();

        switch (command->command_id & GAIA_COMMAND_TYPE_MASK)
        {
        case GAIA_COMMAND_TYPE_CONFIGURATION:
            DEBUG_LOG("appGaiaHandleCommand GAIA_COMMAND_TYPE_CONFIGURATION");
            break;

        case GAIA_COMMAND_TYPE_CONTROL:
            DEBUG_LOG("appGaiaHandleCommand GAIA_COMMAND_TYPE_CONTROL");
            break;

        case GAIA_COMMAND_TYPE_STATUS :
            handled = appGaiaHandleStatusCommand(task, command);
            break;

        case GAIA_COMMAND_TYPE_NOTIFICATION:
            if (command->command_id & GAIA_ACK_MASK)
            {
                DEBUG_LOG("appGaiaHandleCommand NOTIFICATION ACK");
                handled = TRUE;
            }
            else
            {
                DEBUG_LOG("appGaiaHandleCommand NOTIFICATION");
            }
            break;

        default:
            DEBUG_LOG("appGaiaHandleCommand Unexpected GAIA command 0x%x",command->command_id & GAIA_COMMAND_TYPE_MASK);
            break;
        }
    }

    if (!handled)
    {
        appGaiaSendResponse(command->vendor_id, command->command_id, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
    }
}

/*! Handle a Gaia polled status command 

    \return TRUE if handled or FALSE if not */
static bool appGaiaHandleStatusCommand(Task task, const GAIA_UNHANDLED_COMMAND_IND_T *command)
{
    UNUSED(task);
    
    switch (command->command_id)
    {
    case GAIA_COMMAND_GET_APPLICATION_VERSION:
        DEBUG_LOG("appGaiaHandleStatusCommand GAIA_COMMAND_GET_APPLICATION_VERSION");
        return FALSE;

    default:
        DEBUG_LOG("appGaiaHandleStatusCommand Unhandled GAIA_COMMAND 0x%x (%d)",command->command_id,command->command_id);
        return FALSE;
    }
}

/*! Build and Send a Gaia acknowledgement packet */
void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                          uint16 payload_length, uint8 *payload)
{
    appGaiaSendPacket(vendor_id, command_id | GAIA_ACK_MASK, status,
                     payload_length, payload);
}

/*! Build and Send a Gaia protocol packet */
#ifdef GAIA_TEST
bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status, uint16 payload_length, uint8 *payload)
#else
void appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status, uint16 payload_length, uint8 *payload)
#endif
{
    GAIA_TRANSPORT *transport = appGetGaiaTransport();
    
    if(transport) /* Only attempt to send when transport up */
    {
        uint16 packet_length;
        uint8 *packet;
        uint8 flags = GaiaTransportGetFlags(transport);

        DEBUG_LOG("appGaiaSendPacket cmd:%d sts:%d len:%d [flags x%x]",command_id,status,payload_length,flags);

        packet_length = GAIA_HEADER_SIZE + payload_length + 2;
        packet = PanicNull(malloc(packet_length));

        if (packet)
        {
            packet_length = GaiaBuildResponse(packet, flags,
                                              vendor_id, command_id,
                                              status, payload_length, payload);

            GaiaSendPacket(transport, packet_length, packet);
#ifdef GAIA_TEST
            return TRUE;
#endif
        }
    }
#ifdef GAIA_TEST
    return FALSE;
#endif
}


/*! \brief Disconnect any active gaia connection
 */
void appGaiaDisconnect(void)
{
    MessageSend(appGetGaiaTask(), APP_GAIA_INTERNAL_DISCONNECT, NULL);
}


void appGaiaAllowNewConnections(bool allow)
{
    appGetGaia()->connections_allowed = allow;
}


#endif /* INCLUDE_DFU */
