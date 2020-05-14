#include <pmalloc.h>

#include "av_headset_log.h"
#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "audio_forward.h"
#include "tws/attr.h"
#include "tws/audio_forward.h"
#include "tws/peer.h"
#include "apollo.h"
#include "rwfile.h"
#include "online_dbg.h"
#include <ps.h>
#include "tws/sub_phy.h"

uint16 bufferSendUnit = 120;
const uint16 AndroidSendUnit = 120;
const uint16 IosSendUnit = 80;

#ifdef GAIA_TEST


extern void gaiaClearDropAudioSize(void);

extern int gaiaGetDropAudioSize(void);

StarotAttr *attrDecode(uint8 *data, int len);

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

static void gaiaParseDialogStatus(STAROT_DIALOG_STATUS_T* message);
static void gaiaParseDialogSampleRate(STAROT_DIALOG_SAMPLE_RATE_T* message);

static void starotGaiaDialogStopTransport(GAIA_STAROT_IND_T *message);

static void starotGaiaDialogStartTransport(GAIA_STAROT_IND_T *message);


static void gaiaParseCaseStatVer(const GAIA_STAROT_IND_T *message);

static void gaiaGetHeadsetVer(GAIA_STAROT_IND_T *message);//APP主动获取耳机版本
static void gaiaGetDoubleClickSet(GAIA_STAROT_IND_T *message);//App获取设备的耳机的双击配置信息
static void gaiaSetDoubleClickSet(GAIA_STAROT_IND_T *message);//App设置设备的耳机的双击配置信息
static void gaiaAppSetWakeupParameter(GAIA_STAROT_IND_T *mess);
static void gaiaAppGetWakeupParameter(GAIA_STAROT_IND_T *mess);
static void gaiaAppSetWearParameter(GAIA_STAROT_IND_T *mess);
static void gaiaAppGetWearParameter(GAIA_STAROT_IND_T *message);
static void gaiaGetNotifyPowPositionConn(GAIA_STAROT_IND_T *message);//上报电量-位置-连接状态信息
static void gaiaAppGetNotifyPowPositionConncet(GAIA_STAROT_IND_T *message);//App主动获取电量-位置-连接状态信息
static void gaiaAppGetDeviceSN(GAIA_STAROT_IND_T *message);//获取设备的SN信息

static void gaiaSetRequestRecord(GAIA_STAROT_IND_T *message, bool isBegin);//App请求录音
static void gaiaAssistantAwake(GAIA_STAROT_IND_T *message, uint8 type);//ui上报gaia助手唤醒消息
static void gaiaAssistantAudioAppDev(GAIA_STAROT_IND_T *message);//App播放录音
static void gaiaDevRecordStopInfo(GAIA_STAROT_IND_T *message);//接受设备传过来的停止信息
static int gaiaDevUpdateFirmware(GAIA_STAROT_DATA_T *message);//升级固件

static void gaiaControlCallDialog(GAIA_STAROT_IND_T *mess);

static void gaiaControlAcceptDialog(GAIA_STAROT_IND_T *message);

static void gaiaControlRejectDialog(GAIA_STAROT_IND_T *message);

static void gaiaControlPreviousMusic(GAIA_STAROT_IND_T *message);

static void gaiaControlNextMusic(GAIA_STAROT_IND_T *message);

static void gaiaControlVolumeSet(GAIA_STAROT_IND_T *message);

static void gaiaControlHfpMute(bool isMute);

static void gaiaControlGetHfpMute(void);

static void gaiaControlSet8k(GAIA_STAROT_IND_T *message);

static void gaiaSetBondCode(GAIA_STAROT_IND_T *message);

static void gaiaCheckBondCode(GAIA_STAROT_IND_T *message);

static void starotGaiaParseTestCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotGaiaParseAudioCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotSpeedSendIntervalParse(void);

static void gaiaTestProductRest(GAIA_STAROT_IND_T *message);

static void gaiaSendCallNumber(STAROT_DIALOG_CALL_NUMBER_T* message);

static void gaiaTestInEarReadValue(GAIA_STAROT_IND_T *message);

static void gaiaAppTestPacketData(GAIA_STAROT_IND_T *mess);

extern int16 gaiaTestProduct(uint8_t *payload);

// region 升级

static void appGaiaHandlerPrepareDfu(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerPeerPrepareDfuCfm(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerEnterDfu(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerPeerEnterCfm(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerExitDfu(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerPeerExitCfm(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerGetVersionDetail(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerNotifyCommitStatus(GAIA_STAROT_IND_T *message);

// endregion

struct GaiaStarotPrivateData_T {
    Source dialogSpeaker;
    Source dialogMic;
    int audioTransType;
    int speakerDropNum;
    int micDropNum;
    gaia_transport_type gaiaTransportType; // 默认为0 == gaia_transport_none;
    uint8 testSpeedIndex;
    uint16 speedTestSendUnit;
};

static struct GaiaStarotPrivateData_T gaiaStarotPrivateData;

void starotGaiaInit(void) {
    memset(&gaiaStarotPrivateData, 0x00, sizeof(struct GaiaStarotPrivateData_T));
    gaiaStarotPrivateData.speedTestSendUnit = 81;
}

void starotGaiaReset(void) {
//    memset(&gaiaStarotPrivateData, 0x00, sizeof(struct GaiaStarotPrivateData_T));
    gaiaTaskData *this = appGetGaia();
    this->transformAudioFlag = TRANSFORM_NONE;
    this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
    this->needCycleSendAudio = 0;
    disable_audio_forward(TRUE);
    subGaiaNotifyDataClear();
    subGaiaClearCurrentDialog();
}

void starotGaiaDisconnect(void) {
    DEBUG_LOG("starotGaiaDisconnect");
    appTaskListMessageSendId(appGetGaia()->client_list, APP_GAIA_DISCONNECTED);
    starotGaiaReset();
    subGaiaClearConnectUnlock();
    // 判断是否在dfu模式中
    //    是否等待重启
    //       是 忽略，切换耳机升级
    //       否 定时，退出dfu模式
    if (appSmIsInDfuMode()) {
        /// 不需要重启，说明在升级的前期阶段断开连接，需要定时，如果还没有连上，说明需要退出DFU模式
        if (!gProgRunInfo.upgradeNeedReboot) {
            MessageSendLater(appGetUiTask(), APP_UPGRADE_ACCIDENT_DISCONNECT_TIMEOUT, NULL, D_SEC(30));
        }
    }
    subGaiaSetUpgradeGaiaTransform(FALSE);
}

/*
 * 删除日志文件
 */
void starotGaiaHandleDeleteLogFile(GAIA_STAROT_IND_T *message)
{
    UNUSED(message);
    FileCancel(FILE_LOG);
}
/*
 * 上传日志文件到手机APP
 * @return:
 *    TRUE: 成功
 */
#define PER_BAGE_SIZE  40
static uint8 buff[256] = {0};
static uint16 sendtemp = 0;
static uint16 pix = 0;
bool starotGaiaHandleUploadLogFile(GAIA_STAROT_IND_T *message)
{
    static Source file_source;
    static FILE_INDEX findex;
    static uint16 readSize;

    /* 包内数据发送完成 */
    if (sendtemp == 0)
    {
        /* 只会在开始传输的时候执行一次 */
        if ((message->payload[1] & 0x03) == 0)
        {
            findex = FindFileIndex(FILE_LOG);
            if (findex == FILE_NONE)return FALSE;
            PanicNull((file_source = StreamFileSource(findex)));
            readSize = FileRead(file_source,buff);
        }
        else if ((message->payload[1] & 0x03) == 0x03)/* 数据发送过程中 */
        {
            readSize = FileRead(file_source,buff);
        }
        sendtemp = readSize;
        pix = 0;
    }

    /* 把读取的数据包发送走
     * 协议中需要包含发送中和最后一次发送，当readSize == 0就是最后一次发送
     */
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    if (sendtemp > PER_BAGE_SIZE)
    {
        sendtemp -= PER_BAGE_SIZE;
        attr = attrMalloc(&head, PER_BAGE_SIZE + 6);
        attr->payload[5]  = message->payload[1];
        attr->payload[5] &= ~( 1 << 2);/* 数据包内传输 */
        attr->payload[5] &= ~( 1 << 3);
    }
    else
    {
        attr = attrMalloc(&head, sendtemp + 6);
        attr->payload[5]  = message->payload[1];
        attr->payload[5] &= ~( 1 << 3);/* 最后一个数据包内传输 */
        attr->payload[5] |=  ( 1 << 2);
    }
    attr->attr = 0X02;
    attr->payload[0]  = 0;
    attr->payload[1]  = 0;
    attr->payload[2]  = (GAIA_CONNECT_STAROT_UPLOAD_LOG_FILE >> 8) & 0xFF;
    attr->payload[3]  = GAIA_CONNECT_STAROT_UPLOAD_LOG_FILE & 0xFF;
    attr->payload[4]  = message->payload[0];/* session id */
    attr->payload[5] |= (1 << 0);/* 数据传输中 */
    attr->payload[5] |= (1 << 1);
    /* 复制到发送区 */
    if (((attr->payload[5] >> 2)&0x3) == 0)/* 数据包内传输 */
    {
        memcpy(&attr->payload[6],&buff[pix],PER_BAGE_SIZE);
        pix += PER_BAGE_SIZE;
    }
    else if (((attr->payload[5] >> 2)&0x3) == 1)/* 最后一个数据包内传输 */
    {
        memcpy(&attr->payload[6],&buff[pix],sendtemp);
        sendtemp = 0;
    }

    /* 文件发送完成 */
    if (readSize == 0)
    {
        attr = attrMalloc(&head, 6);
        attr->payload[5]  = message->payload[1];
        attr->payload[5] &= ~( 1 << 0);
        attr->payload[5] |=  ( 1 << 1);
        attr->payload[5] |=  ( 1 << 2);
        attr->payload[5] &= ~( 1 << 3);
    }

    if (NULL != head)
    {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
    return TRUE;
}
/*
 * 返回升级固件的校验码,校验码为字符内容累加
 * @message: APP传输的数据类型
 * @return:
 *      FALSE: 失败
 *      TRUE: 成功
 */
bool starotGaiaHandleDataSumCheck(GAIA_STAROT_IND_T *message)
{
    DEBUG_LOG("starotGaiaHandleDataSumCheck");
    uint32 check;
    uint8 ack;

    if (message->payloadLen < 6)
    {
        DEBUG_LOG("message data error");
        return FALSE;
    }
    check = message->payload[2]&0XFF;
    check += (message->payload[3]&0XFF) << 8;
    check += (message->payload[4]&0XFF) << 16;
    check += (message->payload[5]&0XFF) << 24;
    DEBUG_LOG("check = %u",check);
    if (check == gProgRunInfo.check_sum)
    {
        FileWriteOk(1);/* 更新标志位 */
        ack = 0;
    }
    else
        ack = 1;
    /* 返回ack */
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    attr = attrMalloc(&head, 6);
    attr->attr = 0X02;
    attr->payload[0]  = 0;
    attr->payload[1]  = 0;
    attr->payload[2]  = 0x58;
    attr->payload[3]  = 0x01;
    attr->payload[4]  = (message->payload[1])&0XF0;/* session id */
    attr->payload[5]  = ack & 0X01;/* 确定文件是否发送成功 0:成功 1失败 */

    DEBUG_LOG("%x %x",attr->payload[4],attr->payload[5]);
    if (NULL != head)
    {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
    return TRUE;
}
/*
 * 处理APP发来的固件升级数据
 */
bool starotGaiaHandleData(GAIA_STAROT_IND_T *message)
{
    /* 先把收到的数据包，保持起来，再通知ack */
    GAIA_STAROT_DATA_T *message_data = (GAIA_STAROT_DATA_T *)
            PanicUnlessMalloc(sizeof(GAIA_STAROT_DATA_T) + (message->payloadLen > 1 ? (message->payloadLen - 1) : 0));

    message_data->command   = message->command;
    message_data->data_length = message->payloadLen -2;
    message_data->index     = message->payload[0];
    message_data->sessionid = (message->payload[1] >> 4) & 0X0F;
    message_data->type      = (message->payload[1] >> 2) & 0X03;
    message_data->flag      = (message->payload[1]) & 0X03;
    memcpy(message_data->data, &message->payload[2], message_data->data_length);

    if (gaiaDevUpdateFirmware(message_data) < 0)
    {
        DEBUG_LOG("gaiaDevUpdateFirmware error");
        return FALSE;
    }


    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    attr = attrMalloc(&head, 12);
    attr->attr = 0X02;
    attr->payload[0]  = 0;
    attr->payload[1]  = 0;
    attr->payload[2]  = 0x58;
    attr->payload[3]  = 0x00;
    attr->payload[4]  = (message->payload[1]) & 0XF0;/* session id */
    attr->payload[5]  = (message->payload[0] + 1);/* index */
    attr->payload[6]  = 1;
    attr->payload[7] |= ( 1 << 0 );

//    DEBUG_LOG("%x %x",attr->payload[4],attr->payload[5]);
    if (NULL != head)
    {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
    pfree(message_data);
    return TRUE;
}

static uint32 gaia_dbg_cnt = 0;

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message) {
    gaiaTaskData *this = appGetGaia();
    
    /// 内部消息处理
    switch (message->command) {
        case STAROT_NOTIFY_STATUS:
            gaiaGetNotifyPowPositionConn(message);
            break;
        case STAROT_DIALOG_TYPE:
            DEBUG_LOG("STAROT_DIALOG_TYPE");
            break;

        case STAROT_DIALOG_AUDIO_DATA:
            if (gaia_dbg_cnt++ % 100 == 0) {
                DEBUG_LOG("call STAROT_DIALOG_AUDIO_DATA EVENT ");
            }
            starotGaiaSendAudio(NULL);
            break;
    }

    /// 处理来自APP的消息
    switch (message->command) {
        case GAIA_COMMAND_STAROT_FIRST_COMMAND: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_FIRST_COMMAND");
        }
            break;

        case GAIA_COMMAND_STAROT_START_SEND_TIMER: {
            gaiaStarotPrivateData.testSpeedIndex = 0;
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            if (message->payloadLen > 0) {
                gaiaStarotPrivateData.speedTestSendUnit = message->payload[0] + 1;
            } else {
                gaiaStarotPrivateData.speedTestSendUnit = 80 + 1;
            }
            this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            this->needCycleSendAudio = 1;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_START_SEND_TIMER, unit is : %d", gaiaStarotPrivateData.speedTestSendUnit);
            MessageSendLater(&this->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 1);
        }
            break;

        case GAIA_COMMAND_STAROT_STOP_SEND_TIMER: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_STOP_SEND_TIMER");
            this->needCycleSendAudio = 0;
        }
            break;

        case GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_INCOMING_BEGIN replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_BEGIN_TIMEOUT);
            break;

        case GAIA_COMMAND_STAROT_CALL_SETUP_END | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_SETUP_END replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_END_TIMEOUT);
            break;

        case GAIA_COMMAND_STAROT_CALL_ATTR | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_ATTR replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_ATTR_TIMEOUT);
            break;

        case GAIA_COMMAND_STAROT_UPGRADE_NOTIFY_COMMIT_STATUS | GAIA_ACK_MASK:
            appUICancelAllUpgradeTime();
            appPeerSigTxUpgradeCancelNotifyCommitStatusReq(appGetUiTask());
            break;

            /// APP希望接受耳机的音频
        case GAIA_COMMAND_STAROT_START_TRANS_AUDIO_IND:
            starotGaiaDialogStartTransport(message);
            break;

            /// APP希望停止接受音频或压根从头不想要音频数据
        case GAIA_COMMAND_STAROT_END_TRANS_AUDIO_IND:
            starotGaiaDialogStopTransport(message);
            break;

        case GAIA_COMMAND_STAROT_SET_BOND_CODE_IND:
            gaiaSetBondCode(message);
            break;

        case GAIA_COMMAND_STAROT_CHECK_BOND_CODE_IND:
            gaiaCheckBondCode(message);
            break;
    }

    /// 版本信息，状态，双击配置
    switch (message->command) {
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_DOUBLE_CLIENT_CONFIG:
            gaiaGetDoubleClickSet(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_SET_DOUBLE_CLIENT_CONFIG:
            gaiaSetDoubleClickSet(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_VERSION:
            gaiaGetHeadsetVer(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_APPGET_POWER_POSITION_CONNECTION:
            gaiaAppGetNotifyPowPositionConncet(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB:
            gaiaAppSetWakeupParameter(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_APOLLO_WAKEUP_ENB:
            gaiaAppGetWakeupParameter(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB:
            gaiaAppSetWearParameter(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_ADORN_CHEAK_ENB:
            gaiaAppGetWearParameter(message);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_SN:
            gaiaAppGetDeviceSN(message);
            break;
    }
    /// app控制耳机，发送相应蓝牙指令
    switch (message->command) {
        case GAIA_COMMAND_STAROT_CONTROL_CALL_DIALOG:
            gaiaControlCallDialog(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_ACCEPT_DIALOG:
            gaiaControlAcceptDialog(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_REJECT_DIALOG:
            gaiaControlRejectDialog(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_PREVIOUS_MUSIC:
            gaiaControlPreviousMusic(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_NEXT_MUSIC:
            gaiaControlNextMusic(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_VOLUME_SET:
            gaiaControlVolumeSet(message);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_HFP_MUTE:
            gaiaControlHfpMute(TRUE);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_HFP_UNMUTE:
            gaiaControlHfpMute(FALSE);
            break;
        case GAIA_COMMAND_STAROT_CONTROL_GET_HFP_MUTE:
            gaiaControlGetHfpMute();
            break;
        case GAIA_COMMAND_STAROT_CONTROL_8K_ENB:
            gaiaControlSet8k(message);
            break;
    }
    /// 助手52NN
    switch (message->command) {
        case GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START:
            gaiaAssistantAwake(message, message->payload[0]);
            break;
        case GAIA_COMMAND_STAROT_AI_BEGIN_RECORD:
            gaiaSetRequestRecord(message, TRUE);
            break;
        case GAIA_COMMAND_STAROT_AI_END_RECORD:
            gaiaSetRequestRecord(message, FALSE);
            break;
        case GAIA_COMMAND_STAROT_AI_AUDIO_TO_DEVICE:
            gaiaAssistantAudioAppDev(message);
            break;
        case GAIA_CONNECT_STAROT_RECORD_STOP_REPORT:
            gaiaDevRecordStopInfo(message);
            break;
    }
    //固件升级
    switch (message->command) {
        case GAIA_CONNECT_STAROT_UPDATE_FIRMWARE:
            starotGaiaHandleData(message);
            break;
        case GAIA_CONNECT_STAROT_UPDATE_FIRMWARE_MD5:
            starotGaiaHandleDataSumCheck(message);
            break;
        case GAIA_CONNECT_STAROT_UPLOAD_LOG_FILE:
            starotGaiaHandleUploadLogFile(message);
            break;
        case GAIA_CONNECT_STAROT_DELETE_LOG_FILE:
            starotGaiaHandleDeleteLogFile(message);
            break;
    }

    /// 升级
    switch (message->command) {
        case GAIA_COMMAND_STAROT_UPGRADE_PREPARE_DFU:
            appGaiaHandlerPrepareDfu(message);
            break;
        case STAROT_APP_NOTIFY_PEER_UPGRADE_PREPARE_CFM:
            appGaiaHandlerPeerPrepareDfuCfm(message);
            break;
        case GAIA_COMMAND_STAROT_UPGRADE_ENTER_DFU:
            appGaiaHandlerEnterDfu(message);
            break;
        case GAIA_COMMAND_STAROT_UPGRADE_EXIT_DFU:
            /// 强制停止升级流程，后续并断开连接
            appGaiaHandlerExitDfu(message);
            break;
        case GAIA_COMMAND_STAROT_UPGRADE_GET_CURRENT_VERSION_DETAIL:
            appGaiaHandlerGetVersionDetail(message);
            break;

        case STAROT_APP_NOTIFY_PEER_UPGRADE_ENTER_CFM:
            appGaiaHandlerPeerEnterCfm(message);
            break;

        case STAROT_APP_NOTIFY_PEER_UPGRADE_EXIT_CFM:
            appGaiaHandlerPeerExitCfm(message);
            break;

        case STAROT_UI_NOTIFY_COMMIT_STATUS:
            appGaiaHandlerNotifyCommitStatus(message);
            break;
    }

    /// 测试与生产
    switch (message->command) {
        case GAIA_COMMAND_STAROT_TEST_PRODUCT_REST:
            gaiaTestProductRest(message);
            break;
        case GAIA_COMMAND_STAROT_TEST_APOLLO_STATUS:
        {
            uint8_t state = get_apollo_state();
            DEBUG_LOG("get apollo state: %d", state);
            appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_TEST_APOLLO_STATUS,
                                GAIA_STATUS_SUCCESS, 1, &state);
            break;
        }
        case GAIA_COMMAND_STAROT_TEST_ONLINE_DBG:
        {
            online_dbg_cmd_handler(message->payload[0]);
        }
    	case GAIA_COMMAND_STAROT_TEST_IN_EAR_RDVALUE:
        	gaiaTestInEarReadValue(message);
        break;
        case GAIA_COMMAND_STAROT_TEST_PRODUCT:
            gaiaTestProduct(message->payload);
        break;
        case GAIA_COMMAND_STAROT_TEST_PACKET_DATA:
            gaiaAppTestPacketData(message);
        break;
    }
    return TRUE;
}

/// 主要处理设备内部的消息
void starotGaiaDefaultParse(MessageId id, Message message) {
    gaiaTaskData *this = appGetGaia();

    switch (id) {
        case GAIA_STAROT_COMMAND_IND:
            starotGaiaHandleCommand((GAIA_STAROT_IND_T *) message);
            break;
// region 通话速记
        case STAROT_DIALOG_STATUS:
        case STAROT_DIALOG_STATUS_ACTIVE:
            gaiaParseDialogStatus((STAROT_DIALOG_STATUS_T *)message);
            break;

        case STAROT_DIALOG_SAMPLE_RATE:
            gaiaParseDialogSampleRate((STAROT_DIALOG_SAMPLE_RATE_T*)message);
            break;

        case STAROT_DIALOG_CALL_NUMBER:
            gaiaSendCallNumber((STAROT_DIALOG_CALL_NUMBER_T*)message);
            break;
// endregion

        case GAIA_STAROT_MORE_SPACE: {
            starotGaiaParseMessageMoreSpace();
        }
            break;
        case GAIA_STAROT_AUDIO_INTERVAL:
            starotSpeedSendIntervalParse();
            break;

        case STAROT_DIALOG_CALL_BEGIN_TIMEOUT:
            /// 确定现在是否再通话中，如果在，需要继续发送
            if (TRANSFORM_NONE != this->transformAudioFlag) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_BEGIN_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
            break;

        case STAROT_DIALOG_CALL_END_TIMEOUT:
            if (TRANSFORM_NONE == this->transformAudioFlag) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_END_TIMEOUT, need retry send to app");
                if (NULL != this->transport) {
                    StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                    MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
                }
            }
            break;

        case STAROT_DIALOG_CALL_ATTR_TIMEOUT:
            if (TRANSFORM_NONE != this->transformAudioFlag) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_ATTR_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
            break;

        case STAROT_DIALOG_CASE_VER:
        case STAROT_DIALOG_CASE_STAT:
            gaiaParseCaseStatVer(message);
            break;

        default:
            DEBUG_LOG("appGaiaMessageHandler Unknown GAIA message 0x%x (%d)", id, id);
            break;
    }
}

/*
 * 使用事件去驱动让audio_forward去发送数据
 */
void starotGaiaParseMessageMoreSpace(void) {
    gaiaTaskData *this = appGetGaia();

    if (this->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }
    this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;

    if (this->needCycleSendAudio > 0) {
        DEBUG_LOG("now send audio is : %d MessageMoreSpace", this->nowSendAudioPhase);
        MessageSendLater(&this->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 1);
    } else {
        /// 尝试使用messagemorespace这唯一的命令去让他发送消息
        starotNotifyAudioForward(FALSE, 0);
    }
}

void starotNotifyAudioForward(bool st, uint8 flag) {
    gaiaTaskData *this = appGetGaia();

    UNUSED(st);
    if (this->transformAudioFlag <= TRANSFORM_NONE) {
        return;
    }

    if (TRUE == st && flag > 0) {
        if ((flag & GAIA_AUDIO_SPEAKER) > 0 && NULL != gaiaStarotPrivateData.dialogSpeaker) {
            gaiaStarotPrivateData.speakerDropNum += bufferSendUnit;
            SourceDrop(gaiaStarotPrivateData.dialogSpeaker, bufferSendUnit);
        }
        if ((flag & GAIA_AUDIO_MIC) > 0 && NULL != gaiaStarotPrivateData.dialogMic) {
            gaiaStarotPrivateData.micDropNum += bufferSendUnit;
            SourceDrop(gaiaStarotPrivateData.dialogMic, bufferSendUnit);
        }
    }

    {
        GAIA_STAROT_AUDIO_IND_T *starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_IND_T));
        starot->command = STAROT_DIALOG_AUDIO_DATA;
        starot->data = NULL;
        MessageSendLater(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot, (TRUE == st ? 0 : 1));
    }
}

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T *message) {
    UNUSED(message);
    gaiaTaskData *this = appGetGaia();

    /// 不能传输，在source中缓存，如果缓存过多，会丢弃
    if (this->transformAudioFlag < TRANSFORM_CANT) {
        return FALSE;
    }

    if (this->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }
    this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
    //DEBUG_LOG("now send audio is : %d", this->nowSendAudioPhase);
    static uint8 payload[256];
    uint16 flag = 0, pos = 1;

    if (NULL != gaiaStarotPrivateData.dialogSpeaker) {
        int size = SourceSize(gaiaStarotPrivateData.dialogSpeaker);
        if (size >= bufferSendUnit) {
            flag |= GAIA_AUDIO_SPEAKER;
            const uint8 *ptr = (const uint8 *) SourceMap(gaiaStarotPrivateData.dialogSpeaker);
            memcpy(payload + pos, ptr, bufferSendUnit);
            pos += bufferSendUnit;
        }
    }

    if (NULL != gaiaStarotPrivateData.dialogMic) {
        int size = SourceSize(gaiaStarotPrivateData.dialogMic);
        if (size >= bufferSendUnit) {
            flag |= GAIA_AUDIO_MIC;
            const uint8 *ptr = SourceMap(gaiaStarotPrivateData.dialogMic);
            memcpy(payload + pos, ptr, bufferSendUnit);
            pos += bufferSendUnit;
        }
    }

    payload[0] = ((uint8) flag) | ((gaiaStarotPrivateData.testSpeedIndex & 0x0F) << 4);
    gaiaStarotPrivateData.audioTransType = flag;

    if (flag <= 0) {
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        //DEBUG_LOG("now send audio is : %d", this->nowSendAudioPhase);
        return FALSE;
    }

    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, this->transformAudioFlag, 0xfe, pos, payload);
    if (TRUE == st) {
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
        //DEBUG_LOG("now send audio is : %d", this->nowSendAudioPhase);
    } else {
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
        //DEBUG_LOG("now send audio is : %d", this->nowSendAudioPhase);
        //// 分配内存失败，需要使用定时器/数据驱动
        //DEBUG_LOG("send data failed, wait more memory");
    }

    return TRUE;
}

void notifyGaiaDialogSource(Source speaker, Source mic) {
    gaiaStarotPrivateData.dialogSpeaker = speaker;
    gaiaStarotPrivateData.dialogMic = mic;
}

uint8 starotGaiaTransGetAudioType(void) {
    return gaiaStarotPrivateData.audioTransType;
}

void gaiaParseDialogSampleRate(STAROT_DIALOG_SAMPLE_RATE_T* message) {
    DEBUG_LOG("sample rate=%d", message->rate);
    StarotAttr *head = NULL;
    subGaiaCreateDialogSampleRate(&head, message->rate);
    requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_CALL_ATTR, head);
}

void gaiaParseDialogStatus(STAROT_DIALOG_STATUS_T *message) {
    if (NULL == message) return;

    gaiaTaskData *this = appGetGaia();
    hfpState hstate = message->status;
    DEBUG_LOG("gaiaParseDialogStatus hfpStatus is :%04X", hstate);

    if(HFP_STATE_CONNECTED_OUTGOING == hstate || HFP_STATE_CONNECTED_INCOMING == hstate) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, 0xfe, 0, NULL);
        StarotAttr *head = NULL;
        subGaiaCreateDialogDirect(&head, hstate); /// 通话方向
        requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_CALL_ATTR, head);
        this->transformAudioFlag = TRANSFORM_COMING;
    } else if (HFP_STATE_CONNECTED_ACTIVE == hstate) {
        DEBUG_LOG("HFP_STATE_CONNECTED_ACTIVE");
        if (this->transformAudioFlag < TRANSFORM_CANT) {
            this->transformAudioFlag = TRANSFORM_COMING;
        }
        StarotAttr *head = NULL;
        subGaiaCreateDialogNumber(&head);/// 电话号码
        if (!subGaiaGetSampleRate()) { /// 通话采样率
            uint16 rate = appKymeraScoGetRate();
            subGaiaCreateDialogSampleRate(&head, rate);
        }
        requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_CALL_ACTIVE, head);
    } else if (HFP_STATE_CONNECTED_IDLE == hstate) {
        DEBUG_LOG("HFP_STATE_CONNECTED_IDLE");

        StarotAttr *head = NULL;
        subGaiaCreateDialogDropAudio(&head);/// 丢失的字节数
        requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_CALL_ATTR, head);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_INACTIVE, 0xfe, 0, NULL);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_END, 0xfe, 0, NULL);

        if (this->transformAudioFlag > TRANSFORM_CANT) {
            /// todo 建议放到知道电话彻底结束的地方调用，需要考虑多方会话的情况
            DEBUG_LOG("disable audio forward");
            disable_audio_forward(TRUE);
        }
        this->transformAudioFlag = TRANSFORM_NONE;

        subGaiaClearCurrentDialog();
    }
}

static void gaiaSendCallNumber(STAROT_DIALOG_CALL_NUMBER_T* message) {
    DEBUG_LOG("gaiaSendCallNumber number len:%d", message->len);
    if (message->len <= 0) {
        return;
    }
    subGaiaSetCaller(message->number, message->len); /// 缓存电话号码
    StarotAttr *head = NULL;
    subGaiaCreateDialogNumber(&head);/// 电话号码
    requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_CALL_ATTR, head);
}

void gaiaNotifyAudioAcceptStatus(Task task, int command) {
    GAIA_STAROT_IND_T *starot = PanicUnlessNew(GAIA_STAROT_IND_T);
    starot->command = command;
    starot->payloadLen = 0;
    MessageSend(task, GAIA_STAROT_COMMAND_IND, starot);
}

void starotSpeedSendIntervalParse(void) {
    gaiaTaskData *this = appGetGaia();

    if (0 == this->needCycleSendAudio) {
        return;
    }

//    DEBUG_LOG("call starotSpeedSendIntervalParse, nowSendAudioPhase: %d", this->nowSendAudioPhase);
    if (this->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_IDLE) {
        return;
    }

    static uint8 data[201];
    for (int i = 1; i <= 1; ++i) {
        data[0] = i;
        data[1] = gaiaStarotPrivateData.testSpeedIndex;
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe, gaiaStarotPrivateData.speedTestSendUnit, data);
//        DEBUG_LOG("now send speed index is %02x", testSpeedIndex);
    }
    this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
    //DEBUG_LOG("now send audio is : %d", this->nowSendAudioPhase);
//                MessageSendLater(&this->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 10);
}

// { 命令重新发送

#define MAKE_RESEND_COMMAND_WITH_LEN(TYPE, LEN) TYPE *message = (TYPE *) PanicUnlessMalloc(((sizeof(TYPE) + LEN) / 4 * 4) + ((sizeof(TYPE) + LEN) % 4 > 0 ? 4 : 0));

StarotResendCommand *starotResendCommandInit(uint16 command, uint16 len, uint8 *payload) {
    MAKE_RESEND_COMMAND_WITH_LEN(StarotResendCommand, len);
    message->command = command;
    message->len = len;
    if (len > 0) {
        memcpy(message->payload, payload, len);
    }
    return message;
}

StarotResendCommand *starotResendCommandDo(StarotResendCommand *resendCommand, bool stillNeedResend) {
    if (NULL == resendCommand) {
        return NULL;
    }
    appGaiaSendPacket(GAIA_VENDOR_STAROT, resendCommand->command, 0xfe,
                      resendCommand->len, resendCommand->payload);

    if (TRUE == stillNeedResend) {
        return starotResendCommandInit(resendCommand->command, resendCommand->len, resendCommand->payload);
    }

    return NULL;
}

// }

//向app发送盒子状态版本
void gaiaParseCaseStatVer(const GAIA_STAROT_IND_T *message) {

    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    if (message->command == STAROT_DIALOG_CASE_STAT) {
        DEBUG_LOG("call STAROT_DIALOG_CASE_STAT");
        attr = attrMalloc(&head, 4);
        attr->attr = 0X01;
        attr->payload[0] = message->payload[0];
        attr->payload[1] = message->payload[1];
        attr->payload[2] = message->payload[2];
        attr->payload[3] = message->payload[3];
    }

    if (message->command == STAROT_DIALOG_CASE_VER) {
        DEBUG_LOG("call STAROT_DIALOG_CASE_VER");
        attr = attrMalloc(&head, 4);
        attr->attr = 0X02;
        memcpy(&head->payload[0], &message->payload[0], 2);
        memcpy(&head->payload[2], &message->payload[2], 2);
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
//        DEBUG_LOG("len is :%d %p", len, data);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_NOTIFY_CASE_STATUS, 0xfe, len, data);
        attrFree(head, data);
        DEBUG_LOG("call STAROT_DIALOG_CASE_STAT");
    }
}

// APP主动获取盒子耳机版本
void gaiaGetHeadsetVer(GAIA_STAROT_IND_T *message) {
    StarotAttr *body = attrDecode(message->payload, message->payloadLen);
    if (NULL == body) {
        return;
    }

    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;
    if ((body->payload[0] & 0X01) > 0) {   // LEFT
        attr = attrMalloc(&head, 8);
        attr->attr = 0X01;
        uint8 buffer[8] = {0};
        SystemGetVersion(DEV_LEFT, buffer);
        memcpy(attr->payload, buffer, 8);
    }

    if ((body->payload[0] & 0X02) > 0) {
        attr = attrMalloc(&head, 8);
        attr->attr = 0X02;
        uint8 buffer[8] = {0};
        SystemGetVersion(DEV_RIGHT, buffer);
        memcpy(attr->payload, buffer, 8);
    }

    if ((body->payload[0] & 0X04) > 0) {
        attr = attrMalloc(&head, 8);
        attr->attr = 0X04;
        uint8 buffer[8] = {0};
        SystemGetVersion(DEV_CASE, buffer);
        memcpy(attr->payload, buffer, 8);
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_BASE_INFO_GET_VERSION, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_BASE_INFO_GET_VERSION, GAIA_STATUS_SUCCESS, 0, NULL);
    }

    attrFree(body, NULL);
}

// ui主动上报电量-位置-连接状态信息
void gaiaGetNotifyPowPositionConn(GAIA_STAROT_IND_T *message) {
    if (subGaiaNotifyDataIsSame(message->payload)) {
        DEBUG_LOG("gaiaGetNotifyPowPositionConn data same before, so don't need send");
        return;
    } else {
        DEBUG_LOG("gaiaGetNotifyPowPositionConn notify power position connection");
        subGaiaNotifyDataRecord(message->payload);
    }

    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    if (message->payload[0] >= 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X01;
        attr->payload[0] = message->payload[0];
    }
    if (message->payload[1] >= 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        attr->payload[0] = message->payload[1];
    }
    if (message->payload[2] >= 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X03;
        attr->payload[0] = message->payload[2];
    }
    if (message->payload[3] >= 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X04;
        attr->payload[0] = message->payload[3];
    }
    if (message->payload[4] >= 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X05;
        attr->payload[0] = message->payload[4];
    }
    {
        attr = attrMalloc(&head, 12);
        attr->attr = 0X06;
        subGaiaGetBTInfo(attr->payload);
    }
    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_BASE_INFO_NOTIFY_POWER_POSITION_CONNECTION, 0xfe, len, data);
        attrFree(head, data);
    }
}

// App主动获取电量-位置-连接状态信息
void gaiaAppGetNotifyPowPositionConncet(GAIA_STAROT_IND_T *message) {
    subGaiaNotifyDataClear();
    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

//获取设备的SN信息
void gaiaAppGetDeviceSN(GAIA_STAROT_IND_T *message) {
    DEBUG_LOG("gaiaAppGetDeviceSN");
    StarotAttr *head = NULL;
    { /// left
        const uint8 leftSNAttr = 0X01;
        StarotAttr *attr = attrMalloc(&head, DEV_SN_LEN);
        attr->attr = leftSNAttr;
        if (appConfigIsLeft()) {
            memcpy(attr->payload, gFixParam.sn, DEV_SN_LEN);
        } else {
            memcpy(attr->payload, appUIGetPeerSnDetail(), DEV_SN_LEN);
        }
    }
    { /// right
        const uint8 rightSNAttr = 0X02;
        StarotAttr *attr = attrMalloc(&head, DEV_SN_LEN);
        attr->attr = rightSNAttr;
        if (appConfigIsLeft()) {
            memcpy(attr->payload, appUIGetPeerSnDetail(), DEV_SN_LEN);
        } else {
            memcpy(attr->payload, gFixParam.sn, DEV_SN_LEN);
        }
    }
    { /// case
        const uint8 caseSNAttr = 0X03;
        StarotAttr *attr = attrMalloc(&head, DEV_SN_LEN);
        attr->attr = caseSNAttr;
        memcpy(attr->payload, gUserParam.caseSn, DEV_SN_LEN);
    }
    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_INVALID_PARAMETER, 0, NULL);
    }
}

// App获取设备的耳机的双击配置信息
void gaiaGetDoubleClickSet(GAIA_STAROT_IND_T *message) {
    StarotAttr *body = attrDecode(message->payload, message->payloadLen);
    if (NULL == body) {
        return;
    }

    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;
    if ((body->payload[0] & 0X01) > 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X01;
        UserGetKeyFunc(&attr->payload[0], 0);
    }
    if ((body->payload[0] & 0X02) > 0) {
        attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        UserGetKeyFunc(0, &attr->payload[0]);
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    }

    attrFree(body, NULL);
}

// App设置设备的耳机的双击配置信息
void gaiaSetDoubleClickSet(GAIA_STAROT_IND_T *message) {
    StarotAttr *body = attrDecode(message->payload, message->payloadLen);
    if (NULL == body) {
        return;
    }
    uint8 leftKey = 0XFF, rightKey = 0XFF;
    StarotAttr *head = body;
    while (NULL != head) {
        if (0X01 == head->attr) {
            leftKey = head->payload[0];
        } else if (0X02 == head->attr) {
            rightKey = head->payload[0];
        }
        head = head->next;
    }
    if ((0XFF != leftKey) || (0XFF != rightKey)) {
        UserSetKeyFunc(leftKey, rightKey);
    }
    appPeerSigTxSyncDoubleClick(appGetUiTask(), gUserParam.lKeyFunc, gUserParam.rKeyFunc);

    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(body, NULL);
}


void gaiaAppSetWakeupParameter(GAIA_STAROT_IND_T *mess) {
    StarotAttr *pAttr = attrDecode(mess->payload, mess->payloadLen);
    if (NULL == pAttr) {
        return;
    }
    StarotAttr * head = pAttr;
    DEBUG_LOG("gaiaAppSetWakeupParameter");

    //   1           2        3
    // enable(1) + type(1) + time(4)
    MAKE_GAIA_MESSAGE_WITH_LEN(APP_STAROT_WAKEUP_CONFIG_IND, 6);
    message->command = STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB;
    message->messageFrom = MESSAGE_FROM_APP;
    while (NULL != pAttr) {
        if (0X01 == pAttr->attr) {
            message->apollo_enable = pAttr->payload[0];
        } else if (0X02 == pAttr->attr) {
            message->assistant_type = pAttr->payload[0];
        } else if (0X03 == pAttr->attr) {
            memcpy(&(message->timestamp), pAttr->payload, pAttr->len - 1);
        }
        pAttr = pAttr->next;
    }
    MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, mess->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(head, NULL);
}

void gaiaAppGetWakeupParameter(GAIA_STAROT_IND_T *message) {
    StarotAttr *head = NULL;

    DEBUG_LOG("gaiaAppGetWakeupParameter");
    {
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X01;
        attr->payload[0] = gUserParam.apolloEnable;
    }
    {
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        attr->payload[0] = gUserParam.assistantType;
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
}

void gaiaAppSetWearParameter(GAIA_STAROT_IND_T *mess){
    StarotAttr *pAttr = attrDecode(mess->payload, mess->payloadLen);
    if (NULL == pAttr) {
        return;
    }
    StarotAttr *head = pAttr;
    DEBUG_LOG("gaiaAppSetWearParameter");

    MAKE_GAIA_MESSAGE_WITH_LEN(APP_STAROT_WEAR_CONFIG_IND, 0);
    message->command = STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB;

    while (NULL != pAttr) {
        if (0X01 == pAttr->attr) {
            message->wear_enable =  pAttr->payload[0];
        } else if (0X02 == pAttr->attr) {
            memcpy((uint8*)(&(message->timestamp)), pAttr->payload, pAttr->len - 1);
        }
        pAttr = pAttr->next;
    }
    MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, mess->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(head, NULL);
}

void gaiaAppGetWearParameter(GAIA_STAROT_IND_T *message) {
    StarotAttr *head = NULL;

    DEBUG_LOG("gaiaAppGetWearParameter");
    {
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X01;
        attr->payload[0] = gUserParam.sensorEnable;
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
}

// App请求录音
void gaiaSetRequestRecord(GAIA_STAROT_IND_T *message, bool isBegin) {
    gaiaTaskData *this = appGetGaia();

    if (TRUE == isBegin) {
        if (TRANSFORM_NONE == this->transformAudioFlag) {
            this->transformAudioFlag = RECORD_CAN_TRANSFORM;
            gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_AI_USER_START_RECORD);
        }
    } else if (FALSE == isBegin) {
        if (RECORD_CAN_TRANSFORM == this->transformAudioFlag) {
            gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_AI_USER_STOP_RECORD);
            this->transformAudioFlag = TRANSFORM_NONE;
        }
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    forwardSetDataClient(DATA_CLIENT_GAIA);
}



void gaiaAssistantAwake(GAIA_STAROT_IND_T *message, uint8 type)
{
    (void)message;
    StarotAttr *head = NULL;

    DEBUG_LOG("gaiaAssistantAwake");

    {
        StarotAttr *attr = NULL;
        attr = attrMalloc(&head, 1);
        attr->attr = 0X01;
        attr->payload[0] = 0X01;
    }

    {
        StarotAttr *attr = NULL;
        attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        attr->payload[0] = type;
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START, 0xfe, len, data);
        attrFree(head, data);
        DEBUG_LOG("call GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START");
    }
}

void gaiaAssistantAudioAppDev(GAIA_STAROT_IND_T *message) {
    //App播放音频数据
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

// 接受设备传过来的停止信息
void gaiaDevRecordStopInfo(GAIA_STAROT_IND_T *message) {
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    DEBUG_LOG("gaiaDevRecordStopInfo");
    attr = attrMalloc(&head, 1);
    attr->attr = 0X01;
    attr->payload[0] = message->payload[0];

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_CONNECT_STAROT_RECORD_STOP_REPORT, 0xfe, len, data);
        attrFree(head, data);
        DEBUG_LOG("call GAIA_CONNECT_STAROT_RECORD_STOP_REPORT");
    }
}

/*
 * 接收APP设备发送过来的升级数据包，保存为文件即可，以后用于更新盒子固件
 * @message: app传输的数据类型
 * @return:
 *      -1: 失败
 *       0: 成功
*/
int gaiaDevUpdateFirmware(GAIA_STAROT_DATA_T *message)
{
    static FILE_INDEX index;

    if (message->flag == 0X00) /* 开始一次数据传输过程 */
    {
        FileWriteOk(0);
        gProgRunInfo.check_sum = 0;
        /* 打开文件 */
        if ((index = FileOpen(FILE_NAME)) != 0)
        {
            if (FileWrite(index, message->data, message->data_length) == 0)
                return -1;
        }
        else
            return -1;
    }
    else if (message->flag == 0X03) /* 数据发送过程中 */
    {
        if (FileWrite(index, message->data, message->data_length) == 0)
            return -1;
    }
    else if (message->flag == 0X02) /* 结束一次数据传输过程 */
    {
        if (FileWrite(index, message->data, message->data_length) == 0)
            return -1;
        FileClose();
    }
    else /* flag 信息不支持 */
    {
        DEBUG_LOG("flag error = %d",message->flag);
        return -1;
    }

    for(uint16 i = 0; i < message->data_length; i++)
    {
        gProgRunInfo.check_sum += message->data[i];
    }
    if (message->flag == 0X02)
    {

        DEBUG_LOG("check_sum = %u",gProgRunInfo.check_sum);
    }
    return 0;
}

// APP中拨打电话
void gaiaControlCallDialog(GAIA_STAROT_IND_T *mess) {
    StarotAttr *body = attrDecode(mess->payload, mess->payloadLen);
    if (NULL == body) {
        return;
    }
    if (0X01 == body->attr) {
        MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_IND, body->len);
        message->command = GAIA_COMMAND_STAROT_CONTROL_CALL_DIALOG;
        message->payloadLen = body->len;
        memcpy(message->payload, body->payload, body->len);
        MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, mess->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(body, NULL);
}

// APP中接听电话
void gaiaControlAcceptDialog(GAIA_STAROT_IND_T *message) {
    gaiaNotifyAudioAcceptStatus(appGetUiTask(), GAIA_COMMAND_STAROT_CONTROL_ACCEPT_DIALOG);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

// APP中拒接电话
void gaiaControlRejectDialog(GAIA_STAROT_IND_T *message) {
//    appHfpCallReject();
    gaiaNotifyAudioAcceptStatus(appGetUiTask(), GAIA_COMMAND_STAROT_CONTROL_REJECT_DIALOG);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

// APP控制上一首
void gaiaControlPreviousMusic(GAIA_STAROT_IND_T *message) {
    gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_APP_CONTROL_PREVIOUS_TRACK);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

// APP控制下一首
void gaiaControlNextMusic(GAIA_STAROT_IND_T *message) {
    gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_APP_CONTROL_NEXT_TRACK);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

void gaiaControlVolumeSet(GAIA_STAROT_IND_T *message) {
    StarotAttr *body = attrDecode(message->payload, message->payloadLen);
    if (NULL == body) {
        return;
    }
    if (0X01 == body->attr) {
        //

    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

static void gaiaControlHfpMute(bool isMute) {
    if(isMute == TRUE){
        appTestHandsetHfpMute();
        appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CONTROL_HFP_MUTE, GAIA_STATUS_SUCCESS, 0, NULL);
    }
    if(isMute == FALSE){
        appTestHandsetHfpUnMute();
        appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CONTROL_HFP_UNMUTE, GAIA_STATUS_SUCCESS, 0, NULL);
    }
}

static void gaiaControlGetHfpMute(void) {
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    attr = attrMalloc(&head, 1);
    attr->attr = 0X01;
    attr->payload[0] = appHfpIsMuted();

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CONTROL_GET_HFP_MUTE, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
}

static void gaiaControlSet8k(GAIA_STAROT_IND_T *message) {
    Sync8kReq sync8kReq;
    StarotAttr *pAttr = attrDecode(message->payload, message->payloadLen);
    if (NULL == pAttr) {
        return;
    }
    StarotAttr *head = pAttr;

    while (NULL != pAttr) {
        if (0X01 == pAttr->attr) {
            gUserParam.set8kEnb =  pAttr->payload[0];
        } else if (0X02 == pAttr->attr) {
            memcpy((uint8*)(&(gUserParam.set8kModifyTime)), pAttr->payload, pAttr->len - 1);
        }
        pAttr = pAttr->next;
    }
    ParamSaveUserPrm(&gUserParam);
    sync8kReq.set8k = gUserParam.set8kEnb;
    sync8kReq.set8kTime = gUserParam.set8kModifyTime;
    appPeerSigTx8kReq(appGetUiTask(), &sync8kReq);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(head, NULL);
}

static void gaiaSetBondCode(GAIA_STAROT_IND_T *message) {
    if (!appBleIsBond()) {
        uint8 *data = message->payload;
        uint16 advCode = (((uint16) data[0]) << 8) | data[1];
        uint32 bindCode = (((uint32) data[2]) << 24) | (((uint32) data[3]) << 16) | (((uint32) data[4]) << 8) | data[5];
        uint32 timestamp = (((uint32) data[6]) << 24) | (((uint32) data[7]) << 16) | (((uint32) data[8]) << 8) | data[9];
        DEBUG_LOG("gaiaSetBondCode adv:%04X bond:%08X timestamp:%08X", advCode, bindCode, timestamp);
        if (advManagerSaveBleAdvInfo(advCode, bindCode, timestamp)) {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            GattManagerCancelWaitForRemoteClient();
            subGaiaSetConnectUnlock();
            MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
            /// 同步给对方耳机
            appPeerSigTxSyncPair(appGetGaiaTask());
        } else {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_INCORRECT_STATE, 0, NULL);
        }
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
    }
}

static void gaiaCheckBondCode(GAIA_STAROT_IND_T *message) {
    do {
        uint8 *data = message->payload;
        uint32 bindCode = (((uint32) data[0]) << 24) | (((uint32) data[1]) << 16) | (((uint32) data[2]) << 8) | data[3];

        uint8 bleFeature = advManagerSelectFeature();

        if ((appBleIsBond() &&  ADV_FEATURE_PAIR != bleFeature && 0XFF != bleFeature) || (bindCode == 0X20160722)) {
            if ((bindCode == appBleGetBondCode()) || (bindCode == 0X20160722)) {
                subGaiaSetConnectUnlock();
                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
                MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
                break;
            }
        }

        bool isInBondList = appBleFindBondCode(bindCode);
        const uint8 notInBondList = 1, btNotThisCode = 2;
        uint8 rejectReason = (isInBondList ? btNotThisCode : notInBondList);

        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 1, &rejectReason);
    } while (0);
}

void starotGaiaParseCfm(const GAIA_SEND_PACKET_CFM_T *m) {
    gaiaTaskData *this = appGetGaia();

    if (this->needCycleSendAudio > 0) {
        starotGaiaParseTestCfm(m);
    } else {
        starotGaiaParseAudioCfm(m);
    }
}

void starotGaiaParseTestCfm(const GAIA_SEND_PACKET_CFM_T *m) {
    uint8 *packet = m->packet;
    gaiaTaskData *this = appGetGaia();

    uint16 vendor_id = W16(packet + GAIA_OFFS_VENDOR_ID);
    uint16 command_id = W16(packet + GAIA_OFFS_COMMAND_ID);

    if (!CALL_AUDIO_IND(command_id) || vendor_id != GAIA_VENDOR_STAROT) {
        return;
    }

    if (FALSE == m->success) {
        if (gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) {
            this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
            DEBUG_LOG("now send audio is : %d failed", this->nowSendAudioPhase);
            //DEBUG_LOG("now send audio is : %d: %d", this->nowSendAudioPhase);
        } else {
            this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            DEBUG_LOG("now send audio is : %d failed", this->nowSendAudioPhase);
            MessageSendLater(&this->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 10);
        }
    } else {
        DEBUG_LOG("now send audio is : %d success", this->nowSendAudioPhase);
        MessageSendLater(&this->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 1);
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        //DEBUG_LOG("now send audio is : %d: %d", this->nowSendAudioPhase);
        gaiaStarotPrivateData.testSpeedIndex += 1;
    }
}

void starotGaiaParseAudioCfm(const GAIA_SEND_PACKET_CFM_T *m) {
    uint8 *packet = m->packet;
    gaiaTaskData *this = appGetGaia();

    uint16 vendor_id = W16(packet + GAIA_OFFS_VENDOR_ID);
    uint16 command_id = W16(packet + GAIA_OFFS_COMMAND_ID);

    if (GAIA_COMMAND_STAROT_CALL_SETUP_END == command_id && vendor_id == GAIA_VENDOR_STAROT && FALSE == m->success) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_END, 0xfe, 0, NULL);
        return;
    }

    if (!CALL_AUDIO_IND(command_id) || vendor_id != GAIA_VENDOR_STAROT) {
        return;
    }

//    DEBUG_LOG("now send audio is : %d: command:%04x vendor:%04x status:%d", this->nowSendAudioPhase, command_id, vendor_id, m->success);

    if (FALSE == m->success) {
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        if (gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) {
            this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
            //DEBUG_LOG("now send audio is : %d: %d", this->nowSendAudioPhase);
        } else {
            this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            //DEBUG_LOG("now send audio is : %d: %d", this->nowSendAudioPhase);
            starotNotifyAudioForward(FALSE, 0);
        }
    } else {
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        gaiaStarotPrivateData.testSpeedIndex += 1;
        starotNotifyAudioForward(TRUE, starotGaiaTransGetAudioType());
    }
}


void starotGaiaSetTransportType(gaia_transport_type gaiaTransportType) {
    gaiaStarotPrivateData.gaiaTransportType = gaiaTransportType;
    if (gaia_transport_gatt == gaiaStarotPrivateData.gaiaTransportType) {
        bufferSendUnit = IosSendUnit;
    } else {
        bufferSendUnit = AndroidSendUnit;
    }
}

void starotGaiaDialogStartTransport(GAIA_STAROT_IND_T *message) {
    gaiaTaskData *this = appGetGaia();

    if (this->transformAudioFlag == TRANSFORM_COMING) {
        gaiaStarotPrivateData.testSpeedIndex = 0;
        this->transformAudioFlag = DIALOG_CAN_TRANSFORM;
        this->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        DEBUG_LOG("starotGaiaDialogStartTransport call start record dialog");
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_ACCEPT_RECORD);
        DEBUG_LOG("call disable_audio_forward(FALSE);");
        gaiaStarotPrivateData.audioTransType = 0;
        gaiaClearDropAudioSize();
//        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HFP_REQUEST_SCO_AUDIO);
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((this->transformAudioFlag & DIALOG_CAN_TRANSFORM) > 0 ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE),
                        0, NULL);
}

void starotGaiaDialogStopTransport(GAIA_STAROT_IND_T *message) {
    gaiaTaskData *this = appGetGaia();

    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((this->transformAudioFlag == DIALOG_CAN_TRANSFORM) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE), 0, NULL);
    if (this->transformAudioFlag == DIALOG_CAN_TRANSFORM) {
        DEBUG_LOG("starotGaiaDialogStopTransport call stop record dialog");
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_REJECT_RECORD);
        this->transformAudioFlag = TRANSFORM_COMING;
    }
}

// { 测试与生产
void gaiaTestProductRest(GAIA_STAROT_IND_T *message) {
    appSmFactoryReset();
//    appPowerReboot();
    /// todo 清空和设置
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}
// }

// 读取接近光在耳朵中的值
static void gaiaTestInEarReadValue(GAIA_STAROT_IND_T *message)
{
    unsigned short value;
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;
    if ((message->payload[0] & 0X01) > 0) {
        attr = attrMalloc(&head, 2);
        attr->attr = 0X01;
        value = EM20168_Get_psvalue();
        attr->payload[0] = ((value >> 8) & 0xFF);
        attr->payload[1] = (value & 0xFF);
    }

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    }
}

// type: 0:write;
//       1:read size;
//       2:read;
static void gaiaAppTestPacketData(GAIA_STAROT_IND_T *mess)
{
    GAIA_STAROT_ID_T *message = (GAIA_STAROT_ID_T *)
            PanicUnlessMalloc(sizeof(GAIA_STAROT_ID_T) + (mess->payloadLen > 1 ? (mess->payloadLen - 1) : 0));

    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;
    uint16* ptr = (uint16 *)mess->payload;

    message->pskid = (mess->payload[0] << 24) | (mess->payload[1] << 16) | (mess->payload[2] << 8) | mess->payload[3];
    message->type = (mess->payload[4] << 8) | mess->payload[5];
    message->allsize = ((mess->payload[6] << 8) | mess->payload[7])/2;
    message->offset = ((mess->payload[8] << 8) | mess->payload[9])/2;
    message->datasize = ((mess->payload[10] << 8) | mess->payload[11])/2;

    if(message->type == 0){//write

        for(int i = 0; i<message->datasize; i++)
        {
             ptr[6+i] = ((ptr[6+i] >> 8) & 0xFF) | (ptr[6+i] << 8);
        }

        /* 调用写函数 */
        uint16 words_write = PsUpdateAudioKey(message->pskid, &ptr[6], message->datasize, message->offset, message->allsize);

        /* 消息回馈app */
        attr = attrMalloc(&head, 7);
        attr->attr = 0X01;
        attr->payload[0] = (message->pskid >> 24) & 0xFF;
        attr->payload[1] = (message->pskid >> 16) & 0xFF;
        attr->payload[2] = (message->pskid >> 8) & 0xFF;
        attr->payload[3] = (message->pskid >> 0) & 0xFF;
        attr->payload[4] = (uint8)words_write*2;
        attr->payload[5] = (message->offset >> 8) & 0xFF;
        attr->payload[6] = (message->offset >> 0) & 0xFF;
    }
    else if(message->type == 1){//read size
        uint16 key_len = 0;
        PsReadAudioKey(message->pskid, NULL, 0, 0, &key_len);

        attr = attrMalloc(&head, 6);
        attr->attr = 0X02;
        attr->payload[0] = (message->pskid >> 24) & 0xFF;
        attr->payload[1] = (message->pskid >> 16) & 0xFF;
        attr->payload[2] = (message->pskid >> 8) & 0xFF;
        attr->payload[3] = (message->pskid >> 0) & 0xFF;
        attr->payload[4] = (key_len*2 >> 8) & 0xFF;
        attr->payload[5] = key_len*2 & 0xFF;
    }
    else if(message->type == 2){
        uint16 key_length_addr;
        uint16 psKeyBuffer16[message->datasize];
        uint16 all_len = 0;
        PsReadAudioKey(message->pskid, NULL, 0, 0, &all_len);
        uint16 sub = all_len - message->offset;
        uint16 count = (sub > message->datasize) ? message->datasize : sub;
        /* 读取偏移值之后的数据 */
        uint16 key_len = PsReadAudioKey(message->pskid, psKeyBuffer16, count, message->offset, &key_length_addr);

        attr = attrMalloc(&head, 9+key_len*2);
        attr->attr = 0X03;
        attr->payload[0] = (message->pskid >> 24) & 0xFF;
        attr->payload[1] = (message->pskid >> 16) & 0xFF;
        attr->payload[2] = (message->pskid >> 8) & 0xFF;
        attr->payload[3] = (message->pskid >> 0) & 0xFF;
        attr->payload[4] = (uint8)key_len*2;
        attr->payload[5] = (all_len*2 >> 8) & 0xFF;
        attr->payload[6] = (all_len*2 >> 0) & 0xFF;
        attr->payload[7] = (message->offset*2 >> 8) & 0xFF;
        attr->payload[8] = (message->offset*2 >> 0) & 0xFF;
        //此时psKeyBuffer16缓冲区值由于memcpy又使大小端字节交换（顺序正确）
        memcpy(&attr->payload[9], psKeyBuffer16, key_len*2);
    }

    if (NULL != head)
    {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, mess->command, GAIA_STATUS_SUCCESS, len, data);
        attrFree(head, data);
    }
    pfree(message);
}

// region 升级

static bool appGaiaIsCanEnterDfu(void) {
    /// todo 加多点规则

    ///  任何一只耳机在空中，不允许升级
    if (ParamUsingSingle()) {
        if (!appSmIsInCase()) {
            return FALSE;
        }
    } else {
//        if (!(appSmIsInCase() && appPeerSyncIsPeerInCase()))
        if (!(subPhyVirtualStateIsCanConnectCase(subPhyGetVirtualPosition()) &&
              subPhyVirtualStateIsCanConnectCase(appPeerSyncGetPeerVirtualPosition())))
        {
            return FALSE;
        }
    }

    return TRUE;
}

void appGaiaHandlerPrepareDfu(GAIA_STAROT_IND_T *message) {
    UNUSED(message);
    /// 通知左右耳机，进行同步，如果在空中，等待进入case，断开连接，
    appPeerSigTxUpgradePrepareReq(appGetUiTask());
}

/// 用户临时记录准备升级的版本，如果左右耳机都同意升级，则使用
static uint8 tempForUpgradeVersion[DEV_SWVER_LEN];

void appGaiaHandlerEnterDfu(GAIA_STAROT_IND_T *message) {
    DEBUG_LOG("appGaiaHandlerEnterDfu");
    bool isCanEnterDfu = appGaiaIsCanEnterDfu();
    if (isCanEnterDfu) {
        appPeerSigTxUpgradeEnterReq(appGetUiTask());
        memset(tempForUpgradeVersion, 0x00, sizeof(tempForUpgradeVersion));

        StarotAttr *pAttr = attrDecode(message->payload, message->payloadLen);
        if (NULL == pAttr) {
            return;
        }

        StarotAttr * head = pAttr;
        const uint8 VERSION_CMD = 0X01;
        if(VERSION_CMD == pAttr->attr) {
            memcpy(tempForUpgradeVersion, pAttr->payload, DEV_SWVER_LEN);
            DEBUG_LOG("appGaiaHandlerEnterDfu want upgrade software version is :%02X%02X%02X%02X",
                      tempForUpgradeVersion[0], tempForUpgradeVersion[1],
                      tempForUpgradeVersion[2], tempForUpgradeVersion[3]);
        }
        attrFree(head, NULL);
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_INCORRECT_STATE, 0, NULL);
    }
}

static void appGaiaHandlerPeerEnterCfm(GAIA_STAROT_IND_T *message) {
    bool st = message->payload[0];
    if (TRUE == st) {
        memcpy(gProgRunInfo.tempCurrentVer, tempForUpgradeVersion, DEV_SWVER_LEN);
        DEBUG_LOG("appGaiaHandlerPeerEnterCfm want upgrade software version is :%02X%02X%02X", gProgRunInfo.tempCurrentVer[1],
                  gProgRunInfo.tempCurrentVer[2], gProgRunInfo.tempCurrentVer[3]);
        memset(tempForUpgradeVersion, 0x00, sizeof(tempForUpgradeVersion));
        MessageSend(appGetUiTask(), APP_UPGRADE_ENTER_BY_GAIA, NULL);
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_UPGRADE_ENTER_DFU,
            (TRUE == st) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE, 0, NULL);
}

static void appGaiaHandlerPeerPrepareDfuCfm(GAIA_STAROT_IND_T *message) {
    bool st = message->payload[0];
    if (TRUE == st) {
        /// 准备检查连接
        appPeerSigPrepareEnterDfuDo();
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_UPGRADE_PREPARE_DFU,
                        (TRUE == st) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE, 0, NULL);
}


void appGaiaHandlerExitDfu(GAIA_STAROT_IND_T *message) {
    DEBUG_LOG("appGaiaHandlerExitDfu");
    /// 强制停止升级流程，后续并断开连接
    if (appUICanContinueUpgrade()) {
        appPeerSigTxUpgradeExitReq(appGetGaiaTask());
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    }
}

static void appGaiaHandlerPeerExitCfm(GAIA_STAROT_IND_T *message) {
    bool st = message->payload[0];
    if (TRUE == st) {
        MessageSend(appGetUiTask(), APP_UPGRADE_EXIT_BY_GAIA, NULL);
        memset(tempForUpgradeVersion, 0x00, sizeof(tempForUpgradeVersion));
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_UPGRADE_EXIT_DFU,
                        (TRUE == st) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE, 0, NULL);
}

static void appGaiaHandlerGetVersionDetail(GAIA_STAROT_IND_T *message) {
    StarotAttr *head = NULL;
    {
        const uint8 RUN_SF_VERSION_ATTR = 1;
        StarotAttr *attr = attrMalloc(&head, DEV_SWVER_LEN);
        attr->attr = RUN_SF_VERSION_ATTR;
        memcpy(attr->payload, SystemGetCurrentSoftware(), DEV_SWVER_LEN);
    }
    {
        const uint8 UPGRADE_SF_VERSION_ATTR = 2;
        StarotAttr *attr = attrMalloc(&head, DEV_SWVER_LEN);
        attr->attr = UPGRADE_SF_VERSION_ATTR;
        if (0 != memcmp(SystemGetCurrentSoftware(), appCurrVersionGet() + DEV_HWVER_LEN + 1, DEV_SWVER_LEN)) {
            memcpy(attr->payload, appCurrVersionGet() + DEV_HWVER_LEN + 1, DEV_SWVER_LEN);
        }
    }
    {
        const uint8 UPGRADE_STATUS_ATTR = 3;
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = UPGRADE_STATUS_ATTR;
        attr->payload[0] = UpgradeGetState();
    }
    responseGaiaMessageWithAttrHelper(message->command, GAIA_STATUS_SUCCESS, head);
}


static void appGaiaHandlerNotifyCommitStatus(GAIA_STAROT_IND_T *message) {
    DEBUG_LOG("call appGaiaHandlerNotifyCommitStatus, gaia connect status : %d", appGaiaIsConnect());
    if (!(appGaiaIsConnect())) {
        return;
    }

    StarotAttr *head = NULL;
    {
        const uint8 COMMIT_STATUS_ATTR = 1;
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = COMMIT_STATUS_ATTR;
        attr->payload[0] = message->payload[0]; // 1:commit 0 revert
    }
    requestGaiaMessageWithAttrHelper(GAIA_COMMAND_STAROT_UPGRADE_NOTIFY_COMMIT_STATUS, head);
}

//endregion

#endif

// region task数据

subGaiaTaskData  gSubGaiaTaskData;
void subGaiaTaskInit(void) {
    memset(&gSubGaiaTaskData, 0X00, sizeof(gSubGaiaTaskData));
    starotGaiaInit();
    gSubGaiaTaskData.connectLock = SUB_GAIA_CONNECT_LOCK;
}

subGaiaTaskData* subGaiaGetTaskData(void) {
    return &gSubGaiaTaskData;
}

// endregion

// region 联系人信息

void subGaiaClearCurrentDialog(void) {
    DEBUG_LOG("subGaiaClearCurrentDialog");
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->callerLen = 0;
    memset(task->callerNumber, 0X00, sizeof(task->callerNumber));
    task->sampleRate = 0;
}

const uint8* subGaiaGetCaller(uint16* len) {
    DEBUG_LOG("subGaiaGetCaller");
    subGaiaTaskData* task = subGaiaGetTaskData();
    *len = task->callerLen;
    return task->callerNumber;
}

void subGaiaSetCaller(uint8* data, uint16 len) {
    DEBUG_LOG("subGaiaSetCaller");
    subGaiaTaskData* task = subGaiaGetTaskData();
    uint16 s = (len > sizeof(task->callerNumber) ? sizeof(task->callerNumber) : len);
    task->callerLen = s;
    memcpy(task->callerNumber, data, s);
}

void subGaiaSetSampleRate(uint16 sampleRate) {
    DEBUG_LOG("subGaiaSetSampleRate");
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->sampleRate = sampleRate;
}

uint16 subGaiaGetSampleRate(void) {
    DEBUG_LOG("subGaiaGetSampleRate");
    subGaiaTaskData* task = subGaiaGetTaskData();
    return task->sampleRate;
}

// endregion

// region 使用conditionally发送消息

void subGaiaSetConnectUnlock(void) {
    DEBUG_LOG("subGaiaSetConnectUnlock");
    if (appHfpIsCall()) {
        MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_STATUS_ACTIVE);
        MAKE_OBJECT(STAROT_DIALOG_STATUS_T);
        message->status = appHfpGetState();
        MessageSendConditionally(appGetGaiaTask(), STAROT_DIALOG_STATUS_ACTIVE, message, subGaiaGetConnectLock());
    }
    subGaiaNotifyDataClear();
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->connectLock = SUB_GAIA_CONNECT_UNLOCK;
    advManagerStopSpecialVol();
    MessageCancelAll(appGetUiTask(), APP_BLE_SCANABLE_TIMEOUT);
    MessageCancelAll(appGetUiTask(), APP_UPGRADE_ACCIDENT_DISCONNECT_TIMEOUT);
}

void subGaiaClearConnectUnlock(void) {
    DEBUG_LOG("subGaiaClearConnectUnlock");
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->connectLock = SUB_GAIA_CONNECT_LOCK;
}

uint16* subGaiaGetConnectLock(void) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    return &(task->connectLock);
}

bool subGaiaIsConnectLock(void) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    return task->connectLock > 0;
}

// endregion

// region 校验通知的数据，是否一致

bool subGaiaNotifyDataIsSame(uint8* data) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    int end = sizeof(task->notifyData);
    for (int i = 0; i < end; ++i) {
        if (data[i] != task->notifyData[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

void subGaiaNotifyDataClear(void) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    memset(task->notifyData, 0X00, sizeof(task->notifyData));
}

void subGaiaNotifyDataRecord(uint8* data) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    memcpy(task->notifyData, data, sizeof(task->notifyData));
}

// endregion

/// region gaia升级进行中

void subGaiaSetUpgradeGaiaTransform(bool status) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->upgradeTransform = status;
}

bool subGaiaGetUpgradeGaiaTransform(void) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    return task->upgradeTransform;
}

/// endregion

// region 常用条件判断

bool subGaiaIsDialogRecoding(void) {
    gaiaTaskData *this = appGetGaia();

    if (!appHfpIsCall()) {
        DEBUG_LOG("subGaiaIsDialogRecoding appHfpIsCall == FALSE");
        return FALSE;
    }

    if (this->transformAudioFlag != DIALOG_CAN_TRANSFORM) {
        DEBUG_LOG("subGaiaIsDialogRecoding transformAudioFlag : %04X", this->transformAudioFlag);
        return FALSE;
    }

    DEBUG_LOG("subGaiaIsDialogRecoding return TRUE");
    return TRUE;
}

// endregion

// region 通话速记

//// 通话速记相关属性
enum {
    DIALOG_ATTR_CALLER_NUMBER = 0x01, // 通讯号码
    DIALOG_ATTR_DIRECT = 0x02, // 拨出/呼入
    DIALOG_ATTR_DROP_AUDIO_SIZE = 0X06, // 丢弃的音频大小
    DIALOG_ATTR_SAMPLE_RATE = 0X07, //采样率
};

//// 呼入/呼出
enum {
    DIALOG_DIRECT_IN  = 1,  /// 呼入
    DIALOG_DIRECT_OUT = 2,  /// 拨出
};

StarotAttr *subGaiaCreateDialogSampleRate(StarotAttr** parent, uint16 sampleRate) {
    DEBUG_LOG("subGaiaCreateDialogSampleRate sampleRate:%d", sampleRate);
    subGaiaSetSampleRate(sampleRate);

    StarotAttr *attr = PanicNull(attrMalloc(parent, 1));
    attr->attr = DIALOG_ATTR_SAMPLE_RATE;
    if(8000 == sampleRate) {
        attr->payload[0] = 0X01;
    } else if(16000 == sampleRate) {
        attr->payload[0] = 0X02;
    }
    return attr;
}

StarotAttr *subGaiaCreateDialogNumber(StarotAttr** parent) {
    DEBUG_LOG("subGaiaCreateDialogNumber");

    StarotAttr *attr = NULL;
    uint16 numberLen = 0;
    const uint8* numberInfo = subGaiaGetCaller(&numberLen);
    if (numberLen > 0){ /// 电话号码
        attr = PanicNull(attrMalloc(parent, numberLen));
        attr->attr = DIALOG_ATTR_CALLER_NUMBER;
        memcpy(attr->payload, numberInfo, numberLen);
    } else {
        /// 主动获取当前通话信息
        DEBUG_LOG("subGaiaCreateDialogNumber don't have dialog number, so request to phone");
        HfpCurrentCallsRequest(hfp_primary_link);
    }

    return attr;
}

StarotAttr *subGaiaCreateDialogDirect(StarotAttr** parent, hfpState state) {
    if (HFP_STATE_CONNECTED_OUTGOING == state)
        DEBUG_LOG("subGaiaCreateDialogDirect GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, Appending Attr: CALL OUT");
    else
        DEBUG_LOG("subGaiaCreateDialogDirect GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, Appending Attr: CALL IN");
    StarotAttr *attr = PanicNull(attrMalloc(parent, 1));
    attr->attr = DIALOG_ATTR_DIRECT;
    attr->payload[0] = (HFP_STATE_CONNECTED_INCOMING == state) ? DIALOG_DIRECT_IN : DIALOG_DIRECT_OUT;
    return attr;
}

StarotAttr *subGaiaCreateDialogDropAudio(StarotAttr** parent) {
    int num = gaiaGetDropAudioSize();
    DEBUG_LOG("subGaiaCreateDialogNumber:%d", num);
    StarotAttr *attr = PanicNull(attrMalloc(parent, 4));
    attr->attr = DIALOG_ATTR_DROP_AUDIO_SIZE;
    attr->payload[0] = (uint8)((num >> 0) & 0X00FF);
    attr->payload[1] = (uint8)((num >> 8) & 0X00FF);
    attr->payload[2] = (uint8)((num >> 16) & 0X00FF);
    attr->payload[3] = (uint8)((num >> 24) & 0X00FF);
    return attr;
}

// endregion

// region 小工具

void requestGaiaMessageWithAttrHelper(uint16 command, StarotAttr* head) {
    DEBUG_LOG("send message[%04X] to GAIA", command);
    uint16 len = 0;
    uint8 *data = attrEncode(head, &len);
    appGaiaSendPacket(GAIA_VENDOR_STAROT, command, 0xfe, len, data);
    attrFree(head, data);
}

void responseGaiaMessageWithAttrHelper(uint16 command, uint16 status, StarotAttr* head) {
    uint16 len = 0;
    uint8 *data = attrEncode(head, &len);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, command, status, len, data);
    attrFree(head, data);
}

bool appGaiaIsConnectBySpp(void)
{
    return  ((gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) ||
             ((gaiaStarotPrivateData.gaiaTransportType == gaia_transport_spp)));
}


void subGaiaGetBTInfo(uint8* data) {
//    长度	描述
//    1	Hfp是否连接
//    1	A2dp是否连接
//    1	Avrcp是否连接
//    6	经典蓝牙连接地址
//    1	电池模式
//    2	内部状态
    uint8 pos = 0;
    data[pos++] = appDeviceIsHandsetHfpConnected();
    data[pos++] = appDeviceIsHandsetA2dpConnected();
    data[pos++] = appDeviceIsHandsetAvrcpConnected();
    bdaddr bd_addr;
    if (appDeviceGetHandsetBdAddr(&bd_addr)) {
        memset(data + pos, 0x00, 6);
        bdaddr2buffer(&bd_addr, data + pos);
    }
    pos += 6;
    data[pos++] = appSubGetProgRun()->iPowerSaveMode;
    data[pos++] = (appGetState() & 0XFF00) >> 8;
    data[pos++] = (appGetState() & 0X00FF);
}

// endregion
