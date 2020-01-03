
#include "av_headset.h"
#include "peer.h"
#include "av_headset_gaia_starot.h"

#define STAROT_MAKE_SIZE(TYPE) (((sizeof(TYPE)) / 8 * 8) + ((sizeof(TYPE)) % 8 > 0 ? 8 : 0))
#define STAROT_MAKE_MESSAGE(TYPE) TYPE *message = (TYPE *) PanicUnlessMalloc(STAROT_MAKE_SIZE(TYPE))

void appPeerSigTxBleConfigRequest(Task task, const bdaddr *peer_addr, int advCode, int bondCode) {
    if (NULL == peer_addr) {
        return;
    }
#ifdef CONFIG_STAROT_PEERPAIR
    if(ParamUsingSingle()) {    // 检测是否为独立使用
        appBleSetSync(TRUE);
        return;
    }
#endif

    peerSigTaskData *peer_sig = appGetPeerSig();
    STAROT_MAKE_MESSAGE(PEER_SIG_INTERNAL_BLE_CONFIG_REQ_T);
    message->client_task = task;
    message->advCode = advCode;
    message->bondCode = bondCode;
    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_BLE_CONFIG_REQ, message, appPeerSigStartup(peer_addr));
}

/// 将消息发送至对方耳机
void appPeerSigHandleInternalBleConfigRequest(PEER_SIG_INTERNAL_BLE_CONFIG_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalBleConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
            uint8 message[AVRCP_PEER_CMD_BLE_CONFIG_SIZE];
            *((int *) (message + 0)) = req->advCode;
            *((int *) (message + 4)) = req->bondCode;
            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_CMD_BLE_CONFIG,
                                               AVRCP_PEER_CMD_BLE_CONFIG_SIZE, message);
        }
            break;

        default: {
            appPeerSigMsgConnectHandsetConfirmation(req->client_task, peerSigStatusLinkKeyTxFail);
        }
            break;
    }
}

/*! \brief Receive connect handset command. */
bool appPeerSigHandleBleConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    peerSigTaskData *peer_sig = appGetPeerSig();

    DEBUG_LOG("call appPeerSigHandleBleConfigCommand");

    /* validate message */
    if ((ind->size_payload != AVRCP_PEER_CMD_BLE_CONFIG_SIZE)
        || !peer_sig->rx_handset_commands_task) {
        return FALSE;
    } else {
        int *data = (int *) (ind->payload);
        int advCode = data[0];
        int bondCode = data[1];
        /// 是直接存储，还是调用ui
        appBleSetBond(advCode, bondCode);
        appBleSetSync(TRUE);
        return TRUE;
    }
}

void appPeerSigMsgBleConfigConfirmation(Task task, peerSigStatus status) {
    /// todo : 发送消息至指定task，告知同步情况
    UNUSED(task), UNUSED(status);
    DEBUG_LOG("call appPeerSigMsgBleConfigConfirmation:%d", status);
    if (peerSigStatusSuccess == status) {
        appBleSetSync(TRUE);
    }
}

////-------------------------------------------华丽的分割线-------------------------------------------------

void appPeerSigTxDoubleClickConfigRequest(Task task, const bdaddr *peer_addr, uint8 left, uint8 right) {
    peerSigTaskData *peer_sig = appGetPeerSig();
    STAROT_MAKE_MESSAGE(PEER_SIG_INTERNAL_DOBULE_CLICK_CONFIG_REQ_T);
    message->client_task = task;
    message->left = left;
    message->right = right;
    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_DOUBLE_CLICK_SETTING_REQ, message, appPeerSigStartup(peer_addr));
}

void appPeerSigHandleInternalDoubleClickConfigRequest(PEER_SIG_INTERNAL_DOBULE_CLICK_CONFIG_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalBleConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
            uint8 message[AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG_SIZE];
            /* Build data for message  */
            message[0] = req->left;
            message[1] = req->right;
            /* Send the handset address over AVRCP */
            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_CMD_BLE_CONFIG,
                                               AVRCP_PEER_CMD_BLE_CONFIG_SIZE, message);
        }
            break;

        default: {
            appPeerSigMsgConnectHandsetConfirmation(req->client_task, peerSigStatusLinkKeyTxFail);
        }
            break;
    }
}

bool appPeerSigHandleDoubleClickConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    peerSigTaskData *peer_sig = appGetPeerSig();

    /* validate message */
    if ((ind->size_payload != AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG_SIZE) || !peer_sig->rx_handset_commands_task) {
        return FALSE;
    } else {
//        MAKE_MESSAGE(PEER_SIG_CONNECT_HANDSET_IND);
//        message->play_media = !!(ind->payload[0] & AVRCP_PEER_CMD_CONNECT_HANDSET_FLAG_PLAY_MEDIA);
//        /* tell client to connect to handset */
//        MessageSend(peer_sig->rx_handset_commands_task, PEER_SIG_CONNECT_HANDSET_IND, message);
//        DEBUG_LOG("appPeerSigHandleConnectHandsetCommand %d", message->play_media);
        uint8 left = ind->payload[0];
        uint8 right = ind->payload[1];
        UNUSED(left);
        UNUSED(right);
        /// 是直接存储，还是调用ui
        return TRUE;
    }
}

void appPeerSigMsgDoubleClickConfigConfirmation(Task task, peerSigStatus status) {
    /// todo : 发送消息至指定task，告知同步情况
    UNUSED(task), UNUSED(status);
//    MAKE_MESSAGE(PEER_SIG_CONNECT_HANDSET_CFM);
//    message->status = status;
//    MessageSend(task, PEER_SIG_CONNECT_HANDSET_CFM, message);
}

extern UserParam gUserParam;

void appPeerSigTxNormalConfigRequest(Task task, const bdaddr *peer_addr) {
    if (NULL == peer_addr) {
        return;
    }

#ifdef CONFIG_STAROT_PEERPAIR
    if(ParamUsingSingle()) {    // 检测是否为独立使用
        /// todo 设置已经同步
        return;
    }
#endif

    peerSigTaskData *peer_sig = appGetPeerSig();
    STAROT_MAKE_MESSAGE(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T);
    message->client_task = task;
    /// 获取对应的值，并填写
    message->apollo_config = (gUserParam.apolloEnable > 0 ? 1 : 0);
    message->apollo_timestamp = 0X00;
    message->wear_config = (gUserParam.sensorEnable > 0 ? 1 : 0);
    message->wear_timestamp = 0X00;

    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_NORMAL_SETTING_REQ, message, appPeerSigStartup(peer_addr));
}

void appPeerSigHandleInternalNormalConfigRequest(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalNormalConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
            uint8 message[AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE];
            uint8 pos = 0;

            *((uint16*) (message + pos)) = req->apollo_config;
            pos += sizeof(uint16);
            *((uint16*) (message + pos)) = req->wear_timestamp;
            pos += sizeof(uint16);
            *((uint32*) (message + pos)) = req->apollo_timestamp;
            pos += sizeof(uint32);
            *((uint32*) (message + pos)) = req->wear_timestamp;

            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_CMD_NORMAL_CONFIG,
                                               AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE, message);
        }
            break;

        default: {
            appPeerSigMsgConnectHandsetConfirmation(req->client_task, peerSigStatusLinkKeyTxFail);
        }
            break;
    }
}

bool appPeerSigHandleNormalConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    peerSigTaskData *peer_sig = appGetPeerSig();

    DEBUG_LOG("call appPeerSigHandleNormalConfigCommand");

    /* validate message */
    if ((ind->size_payload != AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE)
        || !peer_sig->rx_handset_commands_task) {
        return FALSE;
    } else {
        uint8 *data = (uint8*) (ind->payload);
        uint8 pos = 0;
        { // apollo
            pos = 0;
            uint16 apollo_enable = *(uint16*)(data + pos);
            pos = sizeof(uint16) + sizeof(uint16);
            uint32 apollo_timestamp = *(uint32 *)(data + pos);
            /// 比较当前的时间戳，如果大于保存的时间，则更新当前数据
            UNUSED(apollo_enable);
            UNUSED(apollo_timestamp);

            MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_CONFIG_IND, 0);
            message->payloadLen = 0X01;
            message->messageFrom = MESSAGE_FROM_PEER;
            message->payload[0] = (apollo_enable & 0XFF);
            message->command = STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB;
            MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
        }

        { // wear
            pos = sizeof(uint16);
            uint8 wear_enable = *(uint8*)(data + pos);
            pos = sizeof(uint16) + sizeof(uint16) + sizeof(uint32);
            uint32 wear_timestamp = *(uint32 *)(data + pos);
            /// 比较当前的时间戳，如果大于保存的时间，则更新当前数据
            UNUSED(wear_enable);
            UNUSED(wear_timestamp);

            MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_CONFIG_IND, 0);
            message->payloadLen = 0X01;
            message->messageFrom = MESSAGE_FROM_PEER;
            message->payload[0] = (wear_enable & 0XFF);
            message->command = STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB;
            MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
        }

        return TRUE;
    }
}

void appPeerSigMsgNormalConfigConfirmation(Task task, peerSigStatus status) {
    UNUSED(task);
    UNUSED(status);
}
