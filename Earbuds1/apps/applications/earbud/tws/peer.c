
#include "av_headset.h"
#include "peer.h"
#include "av_headset_gaia_starot.h"

// region 统一注册处理响应

static const PeerUnityParseCenter peerUnityParseCenter[] = {
        {
                AVRCP_PEER_CMD_UPGRADE_CHECK_VERSION, sizeof(CheckVersion),
                &appPeerSigTxUpgradeCheckVersionParse, &appPeerSigTxUpgradeCheckVersionConfirm
        },
        {
                AVRCP_PEER_CMD_UPGRADE_ENTER, 0,
                &appPeerSigTxUpgradeEnterParse, &appPeerSigTxUpgradeEnterConfirm
        },
        {
                AVRCP_PEER_CMD_UPGRADE_EXIT, 0,
                &appPeerSigTxUpgradeExitParse, &appPeerSigTxUpgradeExitConfirm
        },
        {
                AVRCP_PEER_CMD_UPGRADE_CANCEL_NOTIFY_COMMIT_STATUS, 0,
                &appPeerSigTxUpgradeCancelNotifyCommitStatusParse, &appPeerSigTxUpgradeCancelNotifyCommitStatusConfirm
        },
        {
                AVRCP_PEER_CMD_SYNC_VERSION, sizeof(SyncVersionReq),
                &appPeerSigTxSyncVersionParse, &appPeerSigTxSyncVersionConfirm
        },
        {
                AVRCP_PEER_CMD_SYNC_SN, sizeof(SyncSnReq),
                &appPeerSigTxSyncSNParse, &appPeerSigTxSyncSNConfirm
        },
        {
                AVRCP_PEER_CMD_SYNC_8K, sizeof(Sync8kReq),
                &appPeerSigTx8kParse, &appPeerSigTx8kConfirm
        },
        {
                AVRCP_PEER_CMD_UPGRADE_PREPARE, 0,
                appPeerSigTxUpgradePrepareParse, appPeerSigTxUpgradePrepareConfirm
        },
        /// 在此之前添加新的数据
        {0XFFFF, 0X00, NULL, NULL}
};

static bool PeerSendUnityReq(AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *message) {
    bdaddr peer_addr;
    if(FALSE == appDeviceGetPeerBdAddr(&peer_addr)) {
        return FALSE;
    }

    peerSigTaskData *peer_sig = appGetPeerSig();
    MessageSendConditionally(&peer_sig->task, PEER_SIG_INTERNAL_UNITY_REQ, message, appPeerSigStartup(&peer_addr));
    return TRUE;
}

static const PeerUnityParseCenter* internalPeerUnityParseFind(uint16 opid) {
    int pos = 0;
    while (1) {
        if (opid == peerUnityParseCenter[pos].command) {
            return peerUnityParseCenter + pos;
        }
        if (0XFFFF == peerUnityParseCenter[pos].command) {
            return NULL;
        }
        pos += 1;
    }
}

// endregion

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

void appPeerSigTxSyncDoubleClick(Task task, uint8 left, uint8 right) {
    uint8 buffer[2] = {left, right};
    appPeerSigTxDataCommandExt(task, PEERTX_CMD_SYNC_DOUBLE_CLICK, 2, buffer);
}

void appPeerSigTxSyncPair(Task task)          // 同步配对信息
{
    if (ParamUsingSingle()) {
        ParamSyncBlePairSucc();
        return;
    } else {
        appPeerSigTxDataCommandExt(task, PEERTX_CMD_SYNC_BLEPAIR,
                                   sizeof(BlePairInfo) * BLEPAIR_COUNT + 4/*Timer*/,
                                   (uint8 * ) & gBtAddrParam.timeModfy);
    }
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



    case PEERTX_CMD_SYNC_DOUBLE_CLICK:
        UserSetKeyFunc((req->data)[0], (req->data)[1]);
        break;

    case PEERTX_CMD_SYNC_CASEST:
        appUiCaseStatus2FromPeer(req->data);
        break;

    case PEERTX_CMD_SENSOR_ENB:
        gUserParam.sensorEnable = req->data[0];
        ParamSaveUserPrm(&gUserParam);
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


// region 统一发送请求数据

AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *PeerMallocUnityReq(uint16 command, int payloadSize) {
    int s = sizeof(AVRCP_PEER_CMD_INTERNAL_UNITY_REQ) + payloadSize;
    AVRCP_PEER_CMD_INTERNAL_UNITY_REQ * req = (AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *)
            PanicUnlessMalloc((s / 8 * 8) +((s % 8) > 0 ? 8 : 0));
    req->command = command;
    req->length = payloadSize;
    return req;
}

AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* PeerMallocUnityReqWithData(uint16 command, int payloadSize, uint8* data) {
    AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* req = PeerMallocUnityReq(command, payloadSize);
    if (NULL != data && 0 < payloadSize) {
        memcpy(req->data, data, payloadSize);
    }
    return req;
}

void appPeerSigSendUnityRequest(AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *message) {
    DEBUG_LOG("appPeerSigSendUnityRequest, state %u command:%04X len:%d", appPeerSigGetState(), message->command, message->length);

    switch (appPeerSigGetState()) {
        case PEER_SIG_STATE_CONNECTED:
            appPeerSigVendorPassthroughRequest(message->client_task, message->command, message->length, (const uint8*)&(message->data));
            break;

        default:
            appPeerSigParseUnityConfirm(message->command, message->client_task, peerSigStatusLinkKeyTxFail);
            break;
    }
}

bool appPeerSigParseUnityRequest(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind) {
    const PeerUnityParseCenter* parseCenter = internalPeerUnityParseFind(ind->opid);
    if (NULL == parseCenter) {
        DEBUG_LOG("appPeerSigParseUnityRequest don't support this command %04X", ind->opid);
        return FALSE;
    }

    peerSigTaskData* peer_sig = appGetPeerSig();
    /* validate message */
    if ((ind->size_payload != parseCenter->payloadSize) || !peer_sig->rx_handset_commands_task) {
        DEBUG_LOG("appPeerSigParseUnityRequest payloadSize or rx_handset_commands_task is false");
        return FALSE;
    }

    if (NULL == parseCenter->peerCmdParseFun) {
        return FALSE;
    }

    return parseCenter->peerCmdParseFun(ind->payload);
}

void appPeerSigParseUnityConfirm(uint16 opid, Task task, peerSigStatus status) {
    const PeerUnityParseCenter* parseCenter = internalPeerUnityParseFind(opid);
    if (NULL == parseCenter) {
        DEBUG_LOGF("appPeerSigParseUnityConfirm unknown opid:%04X", opid);
        return;
    }
    if (NULL != parseCenter->peerCmdConfirmFun) {
        parseCenter->peerCmdConfirmFun(task, status);
    }
}

// endregion


// region AVRCP_PEER_CMD_UPGRADE_CHECK_VERSION

void appPeerSigTxUpgradeCheckVersionReq(Task task, CheckVersion *checkVersion) {
    AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* req =
            PEER_MALLOC_UNITY_REQ(AVRCP_PEER_CMD_UPGRADE_CHECK_VERSION, CheckVersion, checkVersion);
    req->client_task = task;
    PeerSendUnityReq(req);
}

bool appPeerSigTxUpgradeCheckVersionParse(uint8* payload) {
    CheckVersion * checkVersion = (CheckVersion*)payload;
    const uint8* self = SystemGetCurrentSoftware();
    bool ret = (0 == memcmp(self, checkVersion->softwareVersion, DEV_SWVER_LEN));

    DEBUG_LOG("appPeerSigTxUpgradeCheckVersionParse self:%02X%02X%02X%02X, peer:%02X%02X%02X%02X, ret is :%d",
              self[0], self[1], self[2], self[3], checkVersion->softwareVersion[0],
              checkVersion->softwareVersion[1],checkVersion->softwareVersion[2],
              checkVersion->softwareVersion[3], ret);

    return ret;
}

void appPeerSigTxUpgradeCheckVersionConfirm(Task task, peerSigStatus status) {
    DEBUG_LOG("appPeerSigTxUpgradeCheckVersionConfirm status is : %d", status);
    if (peerSigStatusSuccess == status) {
        MessageSend(task, APP_UPGRADE_COMMIT, NULL);
    } else {
        MessageSendLater(task, APP_CHECK_PEER_FOR_UPDATE, NULL, D_SEC(5));
    }
}

// endregion


// region AVRCP_PEER_CMD_UPGRADE_ENTER

void appPeerSigTxUpgradeEnterReq(Task task) {
    if (ParamUsingSingle()) {
        internalSendStarotAppNotifyPeerUpgradeEnterCfm(TRUE);
    } else {
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req =
                PEER_MALLOC_UNITY_REQ_NODATA(AVRCP_PEER_CMD_UPGRADE_ENTER);
        req->client_task = task;
        PeerSendUnityReq(req);
    }
}

bool appPeerSigTxUpgradeEnterParse(uint8* payload) {
    UNUSED(payload);
    DEBUG_LOG("parse appPeerSigTxUpgradeEnterParse");
    MessageSend(appGetUiTask(), APP_UPGRADE_ENTER_BY_PEER, NULL);
    return TRUE;
}

void appPeerSigTxUpgradeEnterConfirm(Task task, peerSigStatus status) {
    UNUSED(task);
    DEBUG_LOG("parse appPeerSigTxUpgradeEnterConfirm");
    internalSendStarotAppNotifyPeerUpgradeEnterCfm(peerSigStatusSuccess == status ? TRUE : FALSE);
}

// endregion

// region AVRCP_PEER_CMD_UPGRADE_EXIT

void appPeerSigTxUpgradeExitReq(Task task) {
    if (ParamUsingSingle()) {
        internalSendStarotAppNotifyPeerUpgradeExitCfm(TRUE);
    } else {
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req =
                PEER_MALLOC_UNITY_REQ_NODATA(AVRCP_PEER_CMD_UPGRADE_EXIT);
        req->client_task = task;
        PeerSendUnityReq(req);
    }
}

bool appPeerSigTxUpgradeExitParse(uint8* payload) {
    UNUSED(payload);
    DEBUG_LOG("parse appPeerSigTxUpgradeExitParse");
    MessageSend(appGetUiTask(), APP_UPGRADE_EXIT_BY_PEER, NULL);
    return TRUE;
}

void appPeerSigTxUpgradeExitConfirm(Task task, peerSigStatus status) {
    UNUSED(task);
    DEBUG_LOG("confirm appPeerSigTxUpgradeExitConfirm");
    internalSendStarotAppNotifyPeerUpgradeExitCfm(peerSigStatusSuccess == status ? TRUE : FALSE);
}

// endregion

// region AVRCP_PEER_CMD_UPGRADE_CANCEL_NOTIFY_COMMIT_STATUS

static int canSendUpgradeCancelNotifyCommitStatusNum = 3;

void appPeerSigTxUpgradeCancelNotifyCommitStatusReq(Task task) {
    DEBUG_LOG("appPeerSigTxUpgradeCancelNotifyCommitStatusReq");
    if (ParamUsingSingle()) {
        appPeerSigTxUpgradeCancelNotifyCommitStatusConfirm(task, peerSigStatusSuccess);
    } else if (canSendUpgradeCancelNotifyCommitStatusNum > 0) {
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req =
                PEER_MALLOC_UNITY_REQ_NODATA(AVRCP_PEER_CMD_UPGRADE_CANCEL_NOTIFY_COMMIT_STATUS);
        req->client_task = task;
        PeerSendUnityReq(req);
        canSendUpgradeCancelNotifyCommitStatusNum -= 1;
    }
}

bool appPeerSigTxUpgradeCancelNotifyCommitStatusParse(uint8* payload) {
    UNUSED(payload);
    DEBUG_LOG("parse appPeerSigTxUpgradeCancelNotifyCommitStatusParse");
    appUICancelAllUpgradeTime();
    MessageCancelAll(appGetUiTask(), APP_UPGRADE_RESTART_FLAG);
    MessageSend(appGetUiTask(), APP_UPGRADE_RESTART_FLAG, NULL);
    return TRUE;
}

void appPeerSigTxUpgradeCancelNotifyCommitStatusConfirm(Task task, peerSigStatus status) {
    DEBUG_LOG("appPeerSigTxUpgradeCancelNotifyCommitStatusConfirm, status:%d", status);
    if (peerSigStatusSuccess != status) {
        /// 重新发送，尝试几次
        appPeerSigTxUpgradeCancelNotifyCommitStatusReq(task);
    } else {
        MessageCancelAll(appGetUiTask(), APP_UPGRADE_RESTART_FLAG);
        MessageSend(appGetUiTask(), APP_UPGRADE_RESTART_FLAG, NULL);
        if (appSmIsInDfuMode()) {
            appUISetCanEnterDfu(FALSE);
            appSmHandleDfuEnded(TRUE);
        }
    }
}

// endregion


// region 同步左右耳机版本  AVRCP_PEER_CMD_SYNC_VERSION

void appPeerSigTxSyncVersionReq(Task task, bool needFocusReplayVersion) {
    DEBUG_LOG("appPeerSigTxSyncVersionReq");
    if (ParamUsingSingle()) {
        return;
    } else {
        SyncVersionReq syncVersionReq;
        SystemGetCurrentVersion(syncVersionReq.version);
        syncVersionReq.needFocusReplayVersion = needFocusReplayVersion;
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req =
                PEER_MALLOC_UNITY_REQ(AVRCP_PEER_CMD_SYNC_VERSION, SyncVersionReq, (&syncVersionReq));
        req->client_task = task;
        PeerSendUnityReq(req);
    }
}

bool appPeerSigTxSyncVersionParse(uint8* payload) {
    DEBUG_LOG("appPeerSigTxSyncVersionParse");
    SyncVersionReq* syncVersionReq = (SyncVersionReq*)payload;
    appPeerVersionSet(syncVersionReq->version);
    appPeerVersionSyncStatusSet(PeerVersionSyncStatusRecv);
    if (!appPeerVersionSyncStatusIsComplete() || syncVersionReq->needFocusReplayVersion) {
        appPeerVersionSyncSent(FALSE);
    }
    MessageSendLater(appGetUiTask(), APP_CHECK_VERSION, NULL, 500);
    return TRUE;
}

void appPeerSigTxSyncVersionConfirm(Task task, peerSigStatus status) {
    DEBUG_LOG("appPeerSigTxSyncVersionConfirm status :%02X", status);
    if (peerSigStatusSuccess == status) {
        MessageSend(task, APP_CHECK_VERSION, NULL);
        appPeerVersionSyncStatusSet(PeerVersionSyncStatusSent);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_BLE_CONNECTABLE_CHANGE);
    } else {
        appPeerVersionSyncStatusClean(PeerVersionSyncStatusSent);
    }
}

// endregion

// region 同步SN到对端耳机

void appPeerSigTxSyncSNReq(Task task) {
    DEBUG_LOG("appPeerSigTxSyncSNReq");
    if (ParamUsingSingle()) {
        return;
    } else {
        SyncSnReq syncSnReq;
        memcpy(syncSnReq.earbudsSN, gFixParam.sn, DEV_SN_LEN);
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req =
                PEER_MALLOC_UNITY_REQ(AVRCP_PEER_CMD_SYNC_SN, SyncSnReq, (&syncSnReq));
        req->client_task = task;
        PeerSendUnityReq(req);
    }
}

bool appPeerSigTxSyncSNParse(uint8* payload) {
    DEBUG_LOG("appPeerSigTxSyncSNParse");
    SyncSnReq* syncSnReq = (SyncSnReq*)payload;
    appUISetPeerSnDetail(syncSnReq->earbudsSN, DEV_SN_LEN);
    appUISetPeerSnStatus(PEER_SN_SYNC_RECV);
    return TRUE;
}

void appPeerSigTxSyncSNConfirm(Task task, peerSigStatus status) {
    UNUSED(task);
    DEBUG_LOG("appPeerSigTxSyncSNConfirm status is :%d", status);
    if (peerSigStatusSuccess == status) {
        appUISetPeerSnStatus(PEER_SN_SYNC_SENT);
    } else {
        appUIUnSetPeerSnStatus(PEER_SN_SYNC_SENT);
    }
}

//////////////////////////////
void appPeerSigTx8kReq(Task task, Sync8kReq* sync8kReq) {
    AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* req =
            PEER_MALLOC_UNITY_REQ(AVRCP_PEER_CMD_SYNC_8K, Sync8kReq, sync8kReq);
    req->client_task = task;
    PeerSendUnityReq(req);
}

bool appPeerSigTx8kParse(uint8* payload) {
    Sync8kReq * sync8kReq = (Sync8kReq*)payload;
    gUserParam.set8kEnb = sync8kReq->set8k;
    gUserParam.set8kModifyTime = sync8kReq->set8kTime;
    ParamSaveUserPrm(&gUserParam);
    return TRUE;
}

void appPeerSigTx8kConfirm(Task task, peerSigStatus status) {
    (void)task; (void)status;
}

// endregion


// region 通知Peer，app希望设备在断开经典蓝牙之后，进入DFU状态

static void internalSendStarotAppPrepareDfuCfm(bool status) {
    GAIA_STAROT_IND_T *starot = PanicUnlessNew(GAIA_STAROT_IND_T);
    starot->command = STAROT_APP_NOTIFY_PEER_UPGRADE_PREPARE_CFM;
    starot->payloadLen = 1;
    starot->payload[0] = status;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
}

void appPeerSigTxUpgradePrepareReq(Task task) {
    if (ParamUsingSingle()) {
        internalSendStarotAppPrepareDfuCfm(TRUE);
        return;
    } else {
        AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *req = PEER_MALLOC_UNITY_REQ_NODATA(AVRCP_PEER_CMD_UPGRADE_PREPARE);
        req->client_task = task;
        PeerSendUnityReq(req);
    }
}

bool appPeerSigTxUpgradePrepareParse(uint8* payload) {
    UNUSED(payload);
    DEBUG_LOG("parse appPeerSigTxUpgradePrepareParse");
    /// 通过规则去处理
    appPeerSigPrepareEnterDfuDo();
    return TRUE;
}

void appPeerSigTxUpgradePrepareConfirm(Task task, peerSigStatus status) {
    UNUSED(task);
    DEBUG_LOG("confirm appPeerSigTxUpgradePrepareConfirm");
    internalSendStarotAppPrepareDfuCfm(peerSigStatusSuccess == status ? TRUE : FALSE);
}

void appPeerSigPrepareEnterDfuDo(void) {
    appUISetCanEnterDfu(TRUE);
    /// 给用户5分钟时间，让用户把耳机放入充电盒
    MessageSendLater(appGetUiTask(), APP_UPGRADE_CAN_ENTER_DFU_TIMEOUT, NULL, D_SEC(300));
    appConnRulesResetEvent(RULE_EVENT_UPGRADE_PREPARE);
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_UPGRADE_PREPARE);
}

// endregion