
#include "av_headset.h"
#include "peer.h"
#include "av_headset_gaia_starot.h"

#define STAROT_MAKE_SIZE(TYPE) (((sizeof(TYPE)) / 8 * 8) + ((sizeof(TYPE)) % 8 > 0 ? 8 : 0))
#define STAROT_MAKE_MESSAGE(TYPE) TYPE *message = (TYPE *) PanicUnlessMalloc(STAROT_MAKE_SIZE(TYPE))
#define STAROT_MAKE_MESSAGE_WITH_LEN(TYPE, LEN) TYPE *message = (TYPE *) PanicUnlessMalloc((((sizeof(TYPE) + LEN + 3) / 4) * 4))

static void internalSendStarotAppNotifyPeerUpgradeEnterCfm(bool status);
static void internalSendStarotAppNotifyPeerUpgradeExitCfm(bool status);

/// 将消息发送至对方耳机
uint8 g_last_tx_command = 0xFF;     // 记录最后一次发送出去的命令，在CONFIrm函数中来使用
void appPeerSigTxDataCommand(Task task, const bdaddr *peer_addr, uint8 command, uint16 size_payload, const uint8 *payload) {
    if (NULL == peer_addr) {
        return;
    }

    peerSigTaskData *peer_sig = appGetPeerSig();
    STAROT_MAKE_MESSAGE_WITH_LEN(PEER_SIG_INTERNAL_TXDATA_REQ_T, size_payload);

    message->client_task = task;
    message->command     = command;
    message->length      = AVRCP_PEER_CMD_TXDATA_BASE_SIZE + size_payload;
    if(size_payload > 0) {
        memcpy(message->data, payload, size_payload);
    }
    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_TXDATA_REQ, message, appPeerSigStartup(peer_addr));
}

void appPeerSigTxDataCommandExt(Task task, uint8 command, uint16 size_payload, const uint8 *payload)
{
    if (ParamUsingSingle()) {
        return;
    }

    bdaddr peer_addr;
    if(FALSE == appDeviceGetPeerBdAddr(&peer_addr))
        return;

    appPeerSigTxDataCommand(task, &peer_addr,  command, size_payload, payload);
}

void appPeerSigTxDataCommandUi(uint8 command, uint8 payload) {  // 仅一个字节payhload
    appPeerSigTxDataCommandExt(appGetUiTask(), command, 1, &payload);
}

void appPeerSigTxSyncVersion(Task task) {
    uint8 buffer[DEV_HWSWVER_LEN] = {0};
    SystemGetCurrentVersion(buffer);
    appPeerSigTxDataCommandExt(task, PEERTX_CMD_SYNC_VERSION, DEV_HWSWVER_LEN, buffer);
}

void appPeerSigTxSyncDoubleClick(Task task, uint8 left, uint8 right) {
    uint8 buffer[2] = {left, right};
    appPeerSigTxDataCommandExt(task, PEERTX_CMD_SYNC_DOUBLE_CLICK, 2, buffer);
}


void appPeerSigTxUpgradeCheckVersion(Task task, const uint8* data, int len) {
    appPeerSigTxDataCommandExt(task, PEERTX_CMD_UPGRADE_CHECK_VERSION, len, data);
}

void appPeerSigTxUpgradeEnter(Task task) {
    if (ParamUsingSingle()) {
        internalSendStarotAppNotifyPeerUpgradeEnterCfm(TRUE);
    } else {
        appPeerSigTxDataCommandExt(task, PEERTX_CMD_UPGRADE_ENTER, 0, NULL);
    }
}

void appPeerSigTxUpgradeExit(Task task) {
    if (ParamUsingSingle()) {
        internalSendStarotAppNotifyPeerUpgradeExitCfm(TRUE);
    } else {
        appPeerSigTxDataCommandExt(task, PEERTX_CMD_UPGRADE_EXIT, 0, NULL);
    }
}

void appPeerSigTxCancelNotifyCommitStatus(Task task) {
    if (ParamUsingSingle()) {
        return;
    } else {
        appPeerSigTxDataCommandExt(task, PEERTX_CMD_CANCEL_NOTIFY_COMMIT_STATUS, 0, NULL);
    }
}

void appPeerSigTxSyncPair(Task task)          // 同步配对信息
{
    appPeerSigTxDataCommandExt(task, PEERTX_CMD_SYNC_BLEPAIR,
        sizeof(BlePairInfo)*BLEPAIR_COUNT+4/*Timer*/, (uint8*)&gBtAddrParam.timeModfy);
}

bool appUiRecvPeerCommand(PEER_SIG_INTERNAL_TXDATA_REQ_T *req) {              // 接收方： 返回给上层处理
    bool ret = TRUE;
    ProgRIPtr progRun = appSubGetProgRun();

    switch(req->command) {
    case PEERTX_CMD_SYNCGAIA:
        progRun->peerGaiaStat = (req->data[0] == 1) ? 1 : 0;
        break;
    case PEERTX_CMD_WAKEUP_SYS:          /* 副耳机发送系统唤醒命令 [playload[0]:  0:keywakeup, 1:voice wakeup */
        MessageSend(appGetUiTask(), (req->data[0] == 0) ? APP_ASSISTANT_TAP_SYSTEM : APP_ASSISTANT_SYSTEM, 0);
        break;
    case PEERTX_CMD_WAKEUP_APP:
        MessageSend(appGetUiTask(), (req->data[0] == 0) ? APP_ASSISTANT_TAP_AWAKEN : APP_ASSISTANT_AWAKEN, 0);
        break;
    case PEERTX_CMD_SYNC_BLEPAIR:
        ParamSyncBlePair(req->length-2, req->data);
        break;

    case PEERTX_CMD_SYNC_VERSION:
        appPeerVersionSet(req->data);
        appPeerVersionSyncStatusSet(PeerVersionSyncStatusRecv);
        if (!appPeerVersionSyncStatusIsComplete()) {
            appPeerVersionSyncSent();
        }
        MessageSendLater(appGetUiTask(), APP_CHECK_VERSION, NULL, 500);
//        SystemSetVersion(appConfigIsLeft() ? DEV_RIGHT : DEV_LEFT, req->data);
        break;

    case PEERTX_CMD_UPGRADE_CHECK_VERSION: {
        if (!memcmp(SystemGetCurrentSoftware(), req->data, DEV_SWVER_LEN)) {
            ret = FALSE;
        }
    }
        break;

    case PEERTX_CMD_UPGRADE_ENTER: {
        DEBUG_LOG("parse PEERTX_CMD_UPGRADE_ENTER");
        MessageSend(appGetUiTask(), APP_UPGRADE_ENTER_BY_PEER, NULL);
    }
        break;

    case PEERTX_CMD_UPGRADE_EXIT: {
        DEBUG_LOG("parse PEERTX_CMD_UPGRADE_EXIT");
        MessageSend(appGetUiTask(), APP_UPGRADE_EXIT_BY_PEER, NULL);
    }
        break;

    case PEERTX_CMD_CANCEL_NOTIFY_COMMIT_STATUS: {
        DEBUG_LOG("parse PEERTX_CMD_UPGRADE_EXIT");
        appUICancelAllUpgradeTime();
    }
        break;

    case PEERTX_CMD_SYNC_DOUBLE_CLICK:
        UserSetKeyFunc((req->data)[0], (req->data)[1]);
        break;

    case PEERTX_CMD_SYNC_CASEST:
        appUiCaseStatus2FromPeer(req->data);
        break;
    default:
        DEBUG_LOG("Unknown command:%d", req->command);
        ret = FALSE;
        break;
    }

    return ret;
}

///
void appPeerSigTxDataRequest(PEER_SIG_INTERNAL_TXDATA_REQ_T *req) {
    DEBUG_LOG("appPeerSigTxDataRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED:
            g_last_tx_command = req->command;
            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_CMD_TXDATA,
                    req->length, &req->command);
            break;

        default:
            appPeerSigTxDataConfirm(req->client_task, peerSigStatusLinkKeyTxFail);
            break;
    }
}

/*! \brief Receive connect handset command. */
bool appPeerSigRxDataCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    peerSigTaskData *peer_sig = appGetPeerSig();
    int length = ind->payload[1];      // payload0=command payload1:length
    DEBUG_LOG("appPeerSigRxDataCommand(%d-%d)", ind->size_payload, length);

    /* validate message */
    if ((ind->size_payload != length)
        || !peer_sig->rx_handset_commands_task) {
        return FALSE;
    }
    else {
        bool bRet;
        STAROT_MAKE_MESSAGE_WITH_LEN(PEER_SIG_INTERNAL_TXDATA_REQ_T, length);
        memcpy(&message->command, ind->payload, length);
        bRet = appUiRecvPeerCommand(message);
        free(message);
        return bRet;
    }
}

void appPeerSigTxDataConfirm(Task task, peerSigStatus status) {
    UNUSED(task), UNUSED(status);
    DEBUG_LOG("appPeerSigTxDataConfirm:%d, lasttxcmd=%d", status, g_last_tx_command);

    switch(g_last_tx_command) {
    case PEERTX_CMD_SYNC_BLEPAIR:
        if(peerSigStatusSuccess == status)
            ParamSyncBlePairSucc();
        break;

    case PEERTX_CMD_SYNC_VERSION:
        if (peerSigStatusSuccess == status) {
            MessageSend(appGetUiTask(), APP_CHECK_VERSION, NULL);
        }
        break;

    case PEERTX_CMD_UPGRADE_CHECK_VERSION:
        if (peerSigStatusSuccess == status) {
            MessageSend(appGetUiTask(), APP_UPGRADE_COMMIT, NULL);
        } else {
            MessageSendLater(appGetUiTask(), APP_CHECK_PEER_FOR_UPDATE, NULL, D_SEC(5));
        }
        break;

    case PEERTX_CMD_UPGRADE_ENTER:
        DEBUG_LOG("confirm PEERTX_CMD_UPGRADE_ENTER");
        internalSendStarotAppNotifyPeerUpgradeEnterCfm(peerSigStatusSuccess == status ? TRUE : FALSE);
        break;

    case PEERTX_CMD_UPGRADE_EXIT:
        DEBUG_LOG("confirm PEERTX_CMD_UPGRADE_EXIT");
        internalSendStarotAppNotifyPeerUpgradeExitCfm(peerSigStatusSuccess == status ? TRUE : FALSE);
        break;
    }
    g_last_tx_command = 0xFF;
}
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////

#ifdef CONFIG_SINGLE_SYNC_BLE_PAIR


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
#endif

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
    message->assistant_type = gUserParam.assistantType;
    message->apollo_timestamp = gUserParam.assistantModifyTime;
    message->wear_config = (gUserParam.sensorEnable > 0 ? 1 : 0);
    message->wear_timestamp = gUserParam.sensorModifyTime;

    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_NORMAL_SETTING_REQ, message, appPeerSigStartup(peer_addr));
}

void appPeerSigHandleInternalNormalConfigRequest(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalNormalConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
            uint8 message[AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE];
            uint8 pos = 0;
            *((uint8*) (message + pos)) = req->apollo_config;
            pos += sizeof(uint8);
            *((uint8*) (message + pos)) = req->assistant_type;
            pos += sizeof(uint8);
            *((uint8*) (message + pos)) = req->wear_config;
            pos += sizeof(uint8);
            /// unused
            *((uint8*) (message + pos)) = 0;
            pos += sizeof(uint8);
            memcpy(message + pos, &req->apollo_timestamp, sizeof(uint32));
            pos += sizeof(uint32);
            memcpy(message + pos, &req->wear_timestamp, sizeof(uint32));

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
            uint8 apollo_enable = *(uint8*)(data + pos);
            pos = sizeof(uint8);
            uint8 assistant_type = *(uint8*)(data + pos);
            pos = sizeof(uint8) + sizeof(uint8) + sizeof(uint8) + sizeof(uint8);
            uint32 apollo_timestamp = 0;
            memcpy(&apollo_timestamp, (data + pos), sizeof(uint32));
            /// 比较当前的时间戳，如果大于保存的时间，则更新当前数据

            MAKE_GAIA_MESSAGE_WITH_LEN(APP_STAROT_WAKEUP_CONFIG_IND, 0);
            message->command = STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB;
            message->messageFrom = MESSAGE_FROM_PEER;
            message->apollo_enable = (apollo_enable & 0XFF);
            message->assistant_type = (assistant_type & 0XFF);
            message->timestamp = apollo_timestamp;
            MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
        }

        { // wear
            pos = sizeof(uint8) + sizeof(uint8);
            uint8 wear_enable = *(uint8*)(data + pos);
            pos = sizeof(uint8) + sizeof(uint8) + sizeof(uint8) + sizeof(uint8) + sizeof(uint32);
            uint32 wear_timestamp = 0;
            memcpy(&wear_timestamp, (data + pos), sizeof(uint32));
            /// 比较当前的时间戳，如果大于保存的时间，则更新当前数据
            MAKE_GAIA_MESSAGE_WITH_LEN(APP_STAROT_WEAR_CONFIG_IND, 0);
            message->messageFrom = MESSAGE_FROM_PEER;
            message->wear_enable = wear_enable;
            message->timestamp = wear_timestamp;
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

////-------------------------------------------华丽的分割线-------------------------------------------------
void appPeerSigTxDoubleClickWakeupRequest(Task task, const bdaddr *peer_addr, int num) {
    peerSigTaskData *peer_sig = appGetPeerSig();
    STAROT_MAKE_MESSAGE(PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T);
    message->client_task = task;
    if(num == 0)
        MessageSendConditionally(&peer_sig->task,PEER_SIG_INTERNAL_DOUBLE_CLICK_WAKEUP_REQ , message, appPeerSigStartup(peer_addr));
    if(num == 1)
        MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_DOUBLE_CLICK_WAKEUP_SYSTEM_REQ, message, appPeerSigStartup(peer_addr));
}

void appPeerSigHandleInternalDoubleClickWakeupRequest(PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalBleConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
//            uint8 message[AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG_SIZE];
//            /* Build data for message  */
//            message[0] = req->left;
//            message[1] = req->right;
//            memcpy(&message[2], req->peerver, DEV_HWSWVER_LEN);
            /* Send the handset address over AVRCP */
            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_DOUBLE_CLICK_WAKEUP,
                                               0, NULL);
        }
            break;

        default: {
            appPeerSigMsgConnectHandsetConfirmation(req->client_task, peerSigStatusLinkKeyTxFail);
        }
            break;
    }
}

void appPeerSigHandleInternalDoubleClickWakeupSystemRequest(PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T *req) {
    DEBUG_LOG("appPeerSigHandleInternalBleConfigRequest, state %u", appPeerSigGetState());

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED: {
            appPeerSigVendorPassthroughRequest(req->client_task, AVRCP_PEER_DOUBLE_CLICK_WAKEUP_SYSTEM,
                                               0, NULL);
        }
            break;

        default: {
            appPeerSigMsgConnectHandsetConfirmation(req->client_task, peerSigStatusLinkKeyTxFail);
        }
            break;
    }
}

bool appPeerSigHandleDoubleClickWakeupCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    UNUSED(ind);
    MessageSend(&appGetUi()->task, APP_ASSISTANT_TAP_AWAKEN, 0);
    return TRUE;
}

bool appPeerSigHandleDoubleClickWakeupSystemCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    UNUSED(ind);
    MessageSend(&appGetUi()->task, APP_ASSISTANT_TAP_SYSTEM, 0);
    return TRUE;
}
void appPeerSigMsgDoubleClickWakeupConfirmation(Task task, peerSigStatus status) {
    /// todo : 发送消息至指定task，告知同步情况
    UNUSED(task), UNUSED(status);
}

static void internalSendStarotAppNotifyPeerUpgradeEnterCfm(bool status) {
    GAIA_STAROT_IND_T *starot = PanicUnlessNew(GAIA_STAROT_IND_T);
    starot->command = STAROT_APP_NOTIFY_PEER_UPGRADE_ENTER_CFM;
    starot->payloadLen = 1;
    starot->payload[0] = status;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
}

static void internalSendStarotAppNotifyPeerUpgradeExitCfm(bool status) {
    GAIA_STAROT_IND_T *starot = PanicUnlessNew(GAIA_STAROT_IND_T);
    starot->command = STAROT_APP_NOTIFY_PEER_UPGRADE_EXIT_CFM;
    starot->payloadLen = 1;
    starot->payload[0] = status;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
}
