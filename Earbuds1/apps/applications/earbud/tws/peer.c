//
// Created by Administrator on 2019/11/11.
//
#include "av_headset.h"
#include "peer.h"

#define STAROT_MAKE_SIZE(TYPE) (((sizeof(TYPE)) / 8 * 8) + ((sizeof(TYPE)) % 8 > 0 ? 8 : 0))
#define STAROT_MAKE_MESSAGE(TYPE) TYPE *message = (TYPE *) PanicUnlessMalloc(STAROT_MAKE_SIZE(TYPE))

void appPeerSigTxBleConfigRequest(Task task, const bdaddr *peer_addr, int advCode, int bondCode) {
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
            /* Build data for message  */
            *((int *) (message + 0)) = req->advCode;
            *((int *) (message + 4)) = req->bondCode;
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

/*! \brief Receive connect handset command. */
bool appPeerSigHandleBleConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    peerSigTaskData *peer_sig = appGetPeerSig();

    /* validate message */
    if ((ind->size_payload != AVRCP_PEER_CMD_BLE_CONFIG_SIZE)
        || !peer_sig->rx_handset_commands_task) {
        return FALSE;
    } else {
//        MAKE_MESSAGE(PEER_SIG_CONNECT_HANDSET_IND);
//        message->play_media = !!(ind->payload[0] & AVRCP_PEER_CMD_CONNECT_HANDSET_FLAG_PLAY_MEDIA);
//        /* tell client to connect to handset */
//        MessageSend(peer_sig->rx_handset_commands_task, PEER_SIG_CONNECT_HANDSET_IND, message);
//        DEBUG_LOG("appPeerSigHandleConnectHandsetCommand %d", message->play_media);
        int *data = (int *) (ind->payload);
        int advCode = data[0];
        int bondCode = data[1];
        UNUSED(advCode);
        UNUSED(bondCode);
        /// 是直接存储，还是调用ui
        return TRUE;
    }
}

void appPeerSigMsgBleConfigConfirmation(Task task, peerSigStatus status) {
    /// todo : 发送消息至指定task，告知同步情况
    UNUSED(task), UNUSED(status);
//    MAKE_MESSAGE(PEER_SIG_CONNECT_HANDSET_CFM);
//    message->status = status;
//    MessageSend(task, PEER_SIG_CONNECT_HANDSET_CFM, message);
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

