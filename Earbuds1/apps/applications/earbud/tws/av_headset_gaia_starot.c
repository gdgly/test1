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

static void gaiaSetBondCode(GAIA_STAROT_IND_T *message);

static void gaiaCheckBondCode(GAIA_STAROT_IND_T *message);

static void starotGaiaParseTestCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotGaiaParseAudioCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotSpeedSendIntervalParse(void);

static void gaiaTestProductRest(GAIA_STAROT_IND_T *message);

static void gaiaSendCallNumber(STAROT_DIALOG_CALL_NUMBER_T* message);

static void gaiaTestInEarReadValue(GAIA_STAROT_IND_T *message);

extern int16 gaiaTestProduct(uint8_t *payload);

static void appGaiaHandlerEnterDfu(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerPeerEnterCfm(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerExitDfu(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerPeerExitCfm(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerGetVersionDetail(GAIA_STAROT_IND_T *message);
static void appGaiaHandlerNotifyCommitStatus(GAIA_STAROT_IND_T *message);

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
    appGetGaia()->transformAudioFlag = TRANSFORM_NONE;
    appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
    appGetGaia()->needCycleSendAudio = 0;
    /// only test
    disable_audio_forward(TRUE);
    subGaiaNotifyDataClear();
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
            appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            appGetGaia()->needCycleSendAudio = 1;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_START_SEND_TIMER, unit is : %d", gaiaStarotPrivateData.speedTestSendUnit);
            MessageSendLater(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 10);
        }
            break;

        case GAIA_COMMAND_STAROT_STOP_SEND_TIMER: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_STOP_SEND_TIMER");
            appGetGaia()->needCycleSendAudio = 0;
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
    }
    return TRUE;
}

/// 主要处理设备内部的消息
void starotGaiaDefaultParse(MessageId id, Message message) {
    switch (id) {
        case GAIA_STAROT_COMMAND_IND:
            starotGaiaHandleCommand((GAIA_STAROT_IND_T *) message);
            break;
// region 通话速记
        case STAROT_DIALOG_STATUS:
        case STAROT_DIALOG_STATUS_ACTIVE:
            gaiaParseDialogStatus((STAROT_DIALOG_STATUS_T *)message);
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
            if (TRANSFORM_NONE != appGetGaia()->transformAudioFlag) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_BEGIN_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
            break;

        case STAROT_DIALOG_CALL_END_TIMEOUT:
            if (TRANSFORM_NONE == appGetGaia()->transformAudioFlag) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_END_TIMEOUT, need retry send to app");
                if (NULL != appGetGaia()->transport) {
                    StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                    MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
                }
            }
            break;

        case STAROT_DIALOG_CALL_ATTR_TIMEOUT:
            if (TRANSFORM_NONE != appGetGaia()->transformAudioFlag) {
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
    if (appGetGaia()->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }
    appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;

    if (appGetGaia()->needCycleSendAudio > 0) {
        //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
        MessageSend(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL);
    } else {
        /// 尝试使用messagemorespace这唯一的命令去让他发送消息
        starotNotifyAudioForward(FALSE, 0);
    }
}

void starotNotifyAudioForward(bool st, uint8 flag) {
    UNUSED(st);
    if (appGetGaia()->transformAudioFlag <= TRANSFORM_NONE) {
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

    /// 不能传输，在source中缓存，如果缓存过多，会丢弃
    if (appGetGaia()->transformAudioFlag < TRANSFORM_CANT) {
        return FALSE;
    }

    if (appGetGaia()->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }
    appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
    //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
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
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
        return FALSE;
    }

    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, appGetGaia()->transformAudioFlag, 0xfe, pos, payload);
    if (TRUE == st) {
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
        //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
    } else {
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
        //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
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

void gaiaParseDialogStatus(STAROT_DIALOG_STATUS_T *message) {
    if (NULL == message) return;
    hfpState hstate = message->status;
    DEBUG_LOG("gaiaParseDialogStatus hfpStatus is :%04X", hstate);

    if(HFP_STATE_CONNECTED_OUTGOING == hstate || HFP_STATE_CONNECTED_INCOMING == hstate) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, 0xfe, 0, NULL);
        if (HFP_STATE_CONNECTED_OUTGOING == hstate)
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, Appending Attr: CALL OUT");
        else
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN, Appending Attr: CALL IN");

        StarotAttr *head = NULL;
        { /// 拨入/拨出
            const uint8 directAttr = 0X02;
            StarotAttr *attr = PanicNull(attrMalloc(&head, 1));
            attr->attr = directAttr;
            attr->payload[0] = (HFP_STATE_CONNECTED_INCOMING == hstate) ? 0X01 : 0x02;
        }
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ATTR, 0xfe, len, data);
        attrFree(head, data);

        appGetGaia()->transformAudioFlag = TRANSFORM_COMING;
    } else if (HFP_STATE_CONNECTED_ACTIVE == hstate) {
        DEBUG_LOG("HFP_STATE_CONNECTED_ACTIVE");
        if (appGetGaia()->transformAudioFlag < TRANSFORM_CANT) {
            appGetGaia()->transformAudioFlag = TRANSFORM_COMING;
        }
        StarotAttr *head = NULL;
        uint16 numberLen = 0;
        const uint8* numberInfo = subGaiaGetCaller(&numberLen);
        if (numberLen > 0){ /// 电话号码
            const uint8 callerAttr = 0X01;
            StarotAttr *attr = PanicNull(attrMalloc(&head, numberLen));
            attr->attr = callerAttr;
            memcpy(attr->payload, numberInfo, numberLen);
        }
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ACTIVE, 0xfe, len, data);
        attrFree(head, data);
    }else if((8000 == message->status) || (16000 == message->status)) {
        StarotAttr *head = NULL;
        StarotAttr *attr = PanicNull(attrMalloc(&head, 1));
        attr->attr = 0X07;
        if(8000 == message->status)
            attr->payload[0] = 0X01;
        if(16000 == message->status)
            attr->payload[0] = 0X02;

        DEBUG_LOG("sample rate=%d", message->status);
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ATTR, 0xfe, len, data);
        attrFree(head, data);
    } else if (HFP_STATE_CONNECTED_IDLE == hstate) {
        DEBUG_LOG("HFP_STATE_CONNECTED_IDLE");

        StarotAttr *head = NULL;
        { /// 丢失的字节数
            const uint8 dismissAudioSize = 0X06;
            int num = gaiaGetDropAudioSize();
            StarotAttr *attr = PanicNull(attrMalloc(&head, 4));
            attr->attr = dismissAudioSize;
            attr->payload[0] = (uint8)((num >> 0) & 0X00FF);
            attr->payload[1] = (uint8)((num >> 8) & 0X00FF);
            attr->payload[2] = (uint8)((num >> 16) & 0X00FF);
            attr->payload[3] = (uint8)((num >> 24) & 0X00FF);
        }
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_INACTIVE, 0xfe, len, data);
        attrFree(head, data);

        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_END, 0xfe, len, data);

        if (appGetGaia()->transformAudioFlag > TRANSFORM_CANT) {
            /// todo 建议放到知道电话彻底结束的地方调用，需要考虑多方会话的情况
            DEBUG_LOG("disable audio forward");
            disable_audio_forward(TRUE);
        }
        appGetGaia()->transformAudioFlag = TRANSFORM_NONE;

        subGaiaClearCaller();
    }
}

static void gaiaSendCallNumber(STAROT_DIALOG_CALL_NUMBER_T* message) {
    StarotAttr *head = NULL;
    uint16 len = 0;

    DEBUG_LOG("gaiaSendCallNumber");

    // make call number attribute
    StarotAttr *attr = PanicNull(attrMalloc(&head, message->len));
    attr->attr = 0X01;
    if (message->len > 0) {
        memcpy(attr->payload, message->number, message->len);
        /// 缓存电话号码
        subGaiaSetCaller(message->number, message->len);
    }

    // send call number attribute
    uint8 *data = attrEncode(head, &len);
    appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ATTR, 0xfe, len, data);
    attrFree(head, data);
}

void gaiaNotifyAudioAcceptStatus(Task task, int command) {
    GAIA_STAROT_IND_T *starot = PanicUnlessNew(GAIA_STAROT_IND_T);
    starot->command = command;
    starot->payloadLen = 0;
    MessageSend(task, GAIA_STAROT_COMMAND_IND, starot);
}

void starotSpeedSendIntervalParse(void) {
    if (0 == appGetGaia()->needCycleSendAudio) {
        return;
    }

//    DEBUG_LOG("call starotSpeedSendIntervalParse, nowSendAudioPhase: %d", appGetGaia()->nowSendAudioPhase);
    if (appGetGaia()->nowSendAudioPhase != GAIA_TRANSFORM_AUDIO_IDLE) {
        return;
    }

    static uint8 data[201];
    for (int i = 1; i <= 1; ++i) {
        data[0] = i;
        data[1] = gaiaStarotPrivateData.testSpeedIndex;
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe, gaiaStarotPrivateData.speedTestSendUnit, data);
//        DEBUG_LOG("now send speed index is %02x", testSpeedIndex);
    }
    appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_ING;
    //DEBUG_LOG("now send audio is : %d", appGetGaia()->nowSendAudioPhase);
//                MessageSendLater(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 10);
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
    if (TRUE == isBegin) {
        if (TRANSFORM_NONE == appGetGaia()->transformAudioFlag) {
            appGetGaia()->transformAudioFlag = RECORD_CAN_TRANSFORM;
            gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_AI_USER_START_RECORD);
        }
    } else if (FALSE == isBegin) {
        if (RECORD_CAN_TRANSFORM == appGetGaia()->transformAudioFlag) {
            gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_AI_USER_STOP_RECORD);
            appGetGaia()->transformAudioFlag = TRANSFORM_NONE;
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

        if (appBleIsBond() &&  ADV_FEATURE_PAIR != bleFeature && 0XFF != bleFeature) {
            if (bindCode == appBleGetBondCode()) {
                subGaiaSetConnectUnlock();
                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
                MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
                /// 取消超时如果没有发送bondCode断开连接的定时器
                MessageCancelAll(appGetUiTask(), APP_CHECK_GAIA_CONNECTION);
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
    if (appGetGaia()->needCycleSendAudio > 0) {
        starotGaiaParseTestCfm(m);
    } else {
        starotGaiaParseAudioCfm(m);
    }
}

void starotGaiaParseTestCfm(const GAIA_SEND_PACKET_CFM_T *m) {
    uint8 *packet = m->packet;

    uint16 vendor_id = W16(packet + GAIA_OFFS_VENDOR_ID);
    uint16 command_id = W16(packet + GAIA_OFFS_COMMAND_ID);

    if (!CALL_AUDIO_IND(command_id) || vendor_id != GAIA_VENDOR_STAROT) {
        return;
    }

    if (FALSE == m->success) {
        if (gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) {
            appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
            //DEBUG_LOG("now send audio is : %d: %d", appGetGaia()->nowSendAudioPhase);
        } else {
            appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            //DEBUG_LOG("now send audio is : %d: %d", appGetGaia()->nowSendAudioPhase);
            MessageSendLater(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 1);
        }
    } else {
        MessageSend(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL);
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        //DEBUG_LOG("now send audio is : %d: %d", appGetGaia()->nowSendAudioPhase);
        gaiaStarotPrivateData.testSpeedIndex += 1;
    }
}

void starotGaiaParseAudioCfm(const GAIA_SEND_PACKET_CFM_T *m) {
    uint8 *packet = m->packet;
    uint16 vendor_id = W16(packet + GAIA_OFFS_VENDOR_ID);
    uint16 command_id = W16(packet + GAIA_OFFS_COMMAND_ID);

    if (GAIA_COMMAND_STAROT_CALL_SETUP_END == command_id && vendor_id == GAIA_VENDOR_STAROT && FALSE == m->success) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_SETUP_END, 0xfe, 0, NULL);
        return;
    }

    if (!CALL_AUDIO_IND(command_id) || vendor_id != GAIA_VENDOR_STAROT) {
        return;
    }

//    DEBUG_LOG("now send audio is : %d: command:%04x vendor:%04x status:%d", appGetGaia()->nowSendAudioPhase, command_id, vendor_id, m->success);

    if (FALSE == m->success) {
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        if (gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) {
            appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
            //DEBUG_LOG("now send audio is : %d: %d", appGetGaia()->nowSendAudioPhase);
        } else {
            appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
            //DEBUG_LOG("now send audio is : %d: %d", appGetGaia()->nowSendAudioPhase);
            starotNotifyAudioForward(FALSE, 0);
        }
    } else {
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
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
    if (appGetGaia()->transformAudioFlag == TRANSFORM_COMING) {
        gaiaStarotPrivateData.testSpeedIndex = 0;
        appGetGaia()->transformAudioFlag = DIALOG_CAN_TRANSFORM;
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        DEBUG_LOG("starotGaiaDialogStartTransport call start record dialog");
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_ACCEPT_RECORD);
        DEBUG_LOG("call disable_audio_forward(FALSE);");
        gaiaStarotPrivateData.audioTransType = 0;
        gaiaClearDropAudioSize();
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HFP_REQUEST_SCO_AUDIO);
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((appGetGaia()->transformAudioFlag & DIALOG_CAN_TRANSFORM) > 0 ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE),
                        0, NULL);
}

void starotGaiaDialogStopTransport(GAIA_STAROT_IND_T *message) {
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((appGetGaia()->transformAudioFlag == DIALOG_CAN_TRANSFORM) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE), 0, NULL);
    if (appGetGaia()->transformAudioFlag == DIALOG_CAN_TRANSFORM) {
        DEBUG_LOG("starotGaiaDialogStopTransport call stop record dialog");
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_REJECT_RECORD);
        appGetGaia()->transformAudioFlag = TRANSFORM_COMING;
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

#if 0
void gaiaSendDialogActiveStatus(int command, uint8* phone, int len) {
    const uint8 CallerAttr = 0X01;
    StarotAttr *starotAttr = NULL;
    if ((NULL != phone) && (len > 0)) {
        StarotAttr *attr = attrMalloc(&starotAttr, len);
        attr->attr = CallerAttr;
        memcpy(attr->payload, phone, len);
    }
    uint16 attrLen = 0;
    uint8 *attrData = attrEncode(starotAttr, &attrLen);
    appGaiaSendPacket(GAIA_VENDOR_STAROT, command, 0xfe, attrLen, attrData);
    attrFree(starotAttr, attrData);
}
#endif

bool appGaiaIsConnectBySpp(void)
{
    return  ((gaiaStarotPrivateData.gaiaTransportType == gaia_transport_rfcomm) ||
             ((gaiaStarotPrivateData.gaiaTransportType == gaia_transport_spp)));
}


static bool appGaiaIsCanEnterDfu(void) {
    /// todo 加多点规则

    ///  任何一只耳机在空中，不允许升级
    if (ParamUsingSingle()) {
        if (!appSmIsInCase()) {
            return FALSE;
        }
    } else {
        if (!(appSmIsInCase() && appPeerSyncIsPeerInCase())) {
            return FALSE;
        }
    }

    return TRUE;
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

    uint16 len = 0;
    uint8 *data = attrEncode(head, &len);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, len, data);
    attrFree(head, data);
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
    uint16 len = 0;
    uint8 *data = attrEncode(head, &len);
    appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_UPGRADE_NOTIFY_COMMIT_STATUS, 0xfe, len, data);
    attrFree(head, data);
}


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

const uint8* subGaiaGetCaller(uint16* len) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    *len = task->callerLen;
    return task->callerNumber;
}

void subGaiaClearCaller(void) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    task->callerLen = 0;
    memset(task->callerNumber, 0X00, sizeof(task->callerNumber));
}

void subGaiaSetCaller(uint8* data, uint16 len) {
    subGaiaTaskData* task = subGaiaGetTaskData();
    uint16 s = (len > sizeof(task->callerNumber) ? sizeof(task->callerNumber) : len);
    task->callerLen = s;
    memcpy(task->callerNumber, data, s);
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

// region 常用条件判断

bool subGaiaIsDialogRecoding(void) {
    if (!appHfpIsCall()) {
        DEBUG_LOG("subGaiaIsDialogRecoding appHfpIsCall == FALSE");
        return FALSE;
    }

    if (appGetGaia()->transformAudioFlag != DIALOG_CAN_TRANSFORM) {
        DEBUG_LOG("subGaiaIsDialogRecoding transformAudioFlag : %04X", appGetGaia()->transformAudioFlag);
        return FALSE;
    }

    DEBUG_LOG("subGaiaIsDialogRecoding return TRUE");
    return TRUE;
}

// endregion
