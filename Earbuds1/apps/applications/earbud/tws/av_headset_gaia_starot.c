#include <pmalloc.h>

#include "av_headset_log.h"
#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "audio_forward.h"
#include "tws/attr.h"
#include "tws/audio_forward.h"
#include "tws/peer.h"

uint16 bufferSendUnit = 40;

#ifdef GAIA_TEST


extern void gaiaClearDropAudioSize(void);

extern int gaiaGetDropAudioSize(void);

StarotAttr *attrDecode(uint8 *data, int len);

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

static void gaiaParseDialogStatus(GAIA_STAROT_IND_T *message);

static void starotGaiaDialogStopTransport(GAIA_STAROT_IND_T *message);

static void starotGaiaDialogStartTransport(GAIA_STAROT_IND_T *message);

static void gaiaNotifyAudioAcceptStatus(Task task, int command);

static void gaiaParseCaseStatVer(const GAIA_STAROT_IND_T *message);

static void gaiaGetHeadsetVer(GAIA_STAROT_IND_T *message);//APP主动获取耳机版本
static void gaiaGetDoubleClickSet(GAIA_STAROT_IND_T *message);//App获取设备的耳机的双击配置信息
static void gaiaSetDoubleClickSet(GAIA_STAROT_IND_T *message);//App设置设备的耳机的双击配置信息
static void gaiaAppSetParameter(GAIA_STAROT_IND_T *mess, uint16 type);//App设置语言唤醒或者佩戴检测是否使能
static void gaiaAppGetParameter(GAIA_STAROT_IND_T *message, uint16 type);//App获取语言type=1唤醒或type=0者佩戴检测是否使能
static void gaiaGetNotifyPowPositionConn(GAIA_STAROT_IND_T *message);//上报电量-位置-连接状态信息
static void gaiaAppGetNotifyPowPositionConncet(GAIA_STAROT_IND_T *message);//App主动获取电量-位置-连接状态信息

static void gaiaSetRequestRecord(GAIA_STAROT_IND_T *message, bool isBegin);//App请求录音
static void gaiaAssistantAwake(GAIA_STAROT_IND_T *message);//ui上报gaia助手唤醒消息
static void gaiaAssistantAudioAppDev(GAIA_STAROT_IND_T *message);//App播放录音
static void gaiaDevRecordStopInfo(GAIA_STAROT_IND_T *message);//接受设备传过来的停止信息

static void gaiaControlCallDialog(GAIA_STAROT_IND_T *mess);

static void gaiaControlAcceptDialog(GAIA_STAROT_IND_T *message);

static void gaiaControlRejectDialog(GAIA_STAROT_IND_T *message);

static void gaiaControlPreviousMusic(GAIA_STAROT_IND_T *message);

static void gaiaControlNextMusic(GAIA_STAROT_IND_T *message);

static void gaiaControlVolumeSet(GAIA_STAROT_IND_T *message);

static void gaiaSetBondCode(GAIA_STAROT_IND_T *message);

static void gaiaCheckBondCode(GAIA_STAROT_IND_T *message);

static void starotGaiaParseTestCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotGaiaParseAudioCfm(const GAIA_SEND_PACKET_CFM_T *m);

static void starotSpeedSendIntervalParse(void);

static void gaiaTestProductRest(GAIA_STAROT_IND_T *message);

static void gaiaSendDialogActiveStatus(int command, uint8* phone, int len);

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

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message) {
    /// 内部消息处理
    switch (message->command) {
        case STAROT_NOTIFY_STATUS:
            gaiaGetNotifyPowPositionConn(message);
            break;
        case STAROT_DIALOG_TYPE:
            gaiaParseDialogStatus(message);
            break;
        case STAROT_DIALOG_STATUS:
            gaiaParseDialogStatus(message);
            break;
        case STAROT_DIALOG_AUDIO_DATA:
            DEBUG_LOG("call STAROT_DIALOG_AUDIO_DATA EVENT ");
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

        case GAIA_COMMAND_STAROT_CALL_BEGIN | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_BEGIN replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_BEGIN_TIMEOUT);
            break;

        case GAIA_COMMAND_STAROT_CALL_END | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_END replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_END_TIMEOUT);
            break;

        case GAIA_COMMAND_STAROT_CALL_ATTR | GAIA_ACK_MASK:
            DEBUG_LOG("GAIA_COMMAND_STAROT_CALL_ATTR replay");
            MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_ATTR_TIMEOUT);
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
            gaiaAppSetParameter(message, 1);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_APOLLO_WAKEUP_ENB:
            gaiaAppGetParameter(message, 1);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB:
            gaiaAppSetParameter(message, 0);
            break;
        case GAIA_COMMAND_STAROT_BASE_INFO_GET_ADORN_CHEAK_ENB:
            gaiaAppGetParameter(message, 0);
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
    }
    /// 助手52NN
    switch (message->command) {
        case GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START:
            gaiaAssistantAwake(message);
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

    /// 测试与生产
    switch (message->command) {
        case GAIA_COMMAND_STAROT_TEST_PRODUCT_REST:
            gaiaTestProductRest(message);
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

        case GAIA_STAROT_MORE_SPACE: {
//            DEBUG_LOG("Call GAIA_STAROT_MORE_SPACE");
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

    if (appGetGaia()->needCycleSendAudio > 0) {
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
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
//    DEBUG_LOG("transformAudioFlag: %d nowSendAudioPhase is %d", appGetGaia()->transformAudioFlag, appGetGaia()->nowSendAudioPhase);
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

void gaiaParseDialogStatus(GAIA_STAROT_IND_T *message) {
    uint8 status = message->payload[0];
    uint8 dialogIn = 0X01, dialogOut = 0X02, dialogActive = 0X04, dialogInActive = 0X08;

    DEBUG_LOG("call STAROT_DIALOG_STATUS STATUS IS %02x", status);
    StarotAttr *head = NULL;

    char temp[128] = {0};
    int k = 0;
    for (k = 0; k < message->payloadLen; ++k) {
        sprintf(temp + k * 2, "%02X", message->payload[k]);
    }
    DEBUG_LOG("call gaiaParseDialogStatus");
    printf("-hjs-%s\n", temp);

    /// 电话接入,只想通知一次APP电话来的消息
    if ((((status & dialogIn) > 0) && ((appGetGaia()->dialogStatus & dialogIn) < 1))
        || (((status & dialogOut) > 0) && ((appGetGaia()->dialogStatus & dialogOut) < 1))) {
        DEBUG_LOG("Send GAIA_COMMAND_STAROT_CALL_BEGIN");
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_BEGIN, 0xfe, 0, NULL);
//        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_BEGIN, 0, 0);
//        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_BEGIN_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);

        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        attr->payload[0] = ((status & dialogOut) > 0) ? 0X02 : 0X01;

        appGetGaia()->transformAudioFlag = TRANSFORM_COMING;
        appGetGaia()->dialogStatus = appGetGaia()->dialogStatus | dialogIn | dialogOut;
    }

    /// 通话类型
    if (message->command == STAROT_DIALOG_TYPE) {
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X04;
        attr->payload[0] = (status > 0) ? 0X01 : 0X02;
    }


    /// 电话挂断
    bool needSendEnd = FALSE;
    if ((0 == status) || (status & dialogInActive) > 0) {
        DEBUG_LOG("Send GAIA_COMMAND_STAROT_CALL_END");
        {
            StarotAttr *attr = attrMalloc(&head, 4);
            attr->attr = 0X06;
            int num = gaiaGetDropAudioSize();
            attr->payload[0] = (uint8)((num >> 0) & 0X00FF);
            attr->payload[1] = (uint8)((num >> 8) & 0X00FF);
            attr->payload[2] = (uint8)((num >> 16) & 0X00FF);
            attr->payload[3] = (uint8)((num >> 24) & 0X00FF);
        }

//        {
//            StarotAttr *attr = attrMalloc(&head, 1);
//            attr->attr = 0X03;
//        }
        needSendEnd = TRUE;

        if (appGetGaia()->transformAudioFlag > TRANSFORM_CANT) {
            /// todo 建议放到知道电话彻底结束的地方调用，需要考虑多方会话的情况
            disable_audio_forward(TRUE);
            DEBUG_LOG("call disable_audio_forward(TRUE);");
        }
        appGetGaia()->dialogStatus = 0;
        appGetGaia()->transformAudioFlag = TRANSFORM_NONE;
    }

    /// 电话号码
    uint8* phoneInfo = NULL;
    int phoneLen = 0;
    if (message->payloadLen > 2) {
        phoneInfo = message->payload + 2;
        phoneLen = message->payloadLen - 2;
        StarotAttr *attr = attrMalloc(&head, phoneLen);
        attr->attr = 0X01;
        memcpy(attr->payload, phoneInfo, phoneLen);
    }

    /// 发送通话属性
    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ATTR, 0xfe, len, data);
//        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_ATTR, len, data);
//        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_ATTR_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);
        attrFree(head, data);
    }

    /// 电话接通
    if ((status & dialogActive) > 0) {
        gaiaSendDialogActiveStatus(GAIA_COMMAND_STAROT_CALL_ACTIVE, phoneInfo, phoneLen);
    } else if ((status & dialogInActive) > 0) {
        gaiaSendDialogActiveStatus(GAIA_COMMAND_STAROT_CALL_INACTIVE, phoneInfo, phoneLen);
    }

    if (TRUE == needSendEnd) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);
//        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_END, 0, 0);
//        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_END_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);
    }
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
    gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_RECORD_RETURN_THREE_POWER);
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
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
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(body, NULL);
}

//App设置语言唤醒或者佩戴检测是否使能
void gaiaAppSetParameter(GAIA_STAROT_IND_T *mess, uint16 type){
    StarotAttr *body = attrDecode(mess->payload, mess->payloadLen);
    if (NULL == body) {
        return;
    }
    if (0X01 == body->attr) {
        MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_IND, body->len);
        message->payloadLen = body->len;
        message->payload[0] = body->payload[0];
        if(1 == type)
            message->command = STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB;
        if(0 == type)
            message->command = STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB;
        MessageSend(appGetUiTask(), GAIA_STAROT_COMMAND_IND, message);
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, mess->command, GAIA_STATUS_SUCCESS, 0, NULL);
    attrFree(body, NULL);
}

//App获取语言唤醒或者佩戴检测是否使能
void gaiaAppGetParameter(GAIA_STAROT_IND_T *message, uint16 type){
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    DEBUG_LOG("gaiaAppGetApolloWakeupEnb");
    attr = attrMalloc(&head, 1);
    attr->attr = 0X01;
    if(1 == type)
        attr->payload[0] = gUserParam.apolloEnable;
    if(0 == type)
        attr->payload[0] = gUserParam.sensorEnable;

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

void gaiaAssistantAwake(GAIA_STAROT_IND_T *message)
{
    StarotAttr *head = NULL;
    StarotAttr *attr = NULL;

    DEBUG_LOG("gaiaDevRecordStopInfo");
    attr = attrMalloc(&head, 1);
    attr->attr = 0X01;
    attr->payload[0] = 0X01;

    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, message->command, 0xfe, len, data);
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

// APP中拨打电话
void gaiaControlCallDialog(GAIA_STAROT_IND_T *mess) {
    StarotAttr *body = attrDecode(mess->payload, mess->payloadLen);
    if (NULL == body) {
        return;
    }
    if (0X01 == body->attr) {
        uint16 length = 0;
        /// 电话号码长度不会大于16
        const int MaxPhoneNuberLen = 16;
        if (body->len <= MaxPhoneNuberLen) {
            length = body->len;
        } else {
            length = MaxPhoneNuberLen;
        }

        MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_IND, length);
        message->command = GAIA_COMMAND_STAROT_CONTROL_CALL_DIALOG;
        message->payloadLen = length;
        memcpy(message->payload, body->payload, length);
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

void gaiaControlPreviousMusic(GAIA_STAROT_IND_T *message) {
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
}

void gaiaControlNextMusic(GAIA_STAROT_IND_T *message) {
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

static void gaiaSetBondCode(GAIA_STAROT_IND_T *message) {
    if (!appBleIsBond()) {
        uint8 *data = message->payload;
        uint16 advCode = (((uint16) data[0]) << 8) | data[1];
        uint32 bindCode = (((uint32) data[2]) << 24) | (((uint32) data[3]) << 16) | (((uint32) data[4]) << 8) | data[5];
        appBleSetBond(advCode, bindCode);
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_RECORD_RETURN_THREE_POWER);
        GattManagerCancelWaitForRemoteClient();
        /// 查找对方地址
        bdaddr peer_addr;
        if (appDeviceGetPeerBdAddr(&peer_addr)) {
            appPeerSigTxBleConfigRequest(appGetGaiaTask(), &peer_addr, advCode, (int) bindCode);
        }
        /// 取消超时如果没有发送bondCode断开连接的定时器
    } else {
        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
    }
}

static void gaiaCheckBondCode(GAIA_STAROT_IND_T *message) {
    do {
        if (appBleIsBond()) {
            uint8 *data = message->payload;
            uint32 bindCode = (((uint32) data[0]) << 24) | (((uint32) data[1]) << 16) | (((uint32) data[2]) << 8) | data[3];
            if (bindCode == appBleGetBondCode()) {
                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
                gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_RECORD_RETURN_THREE_POWER);
                /// 取消超时如果没有发送bondCode断开连接的定时器
                break;
            }
        }

        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
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

    if (GAIA_COMMAND_STAROT_CALL_END == command_id && vendor_id == GAIA_VENDOR_STAROT && FALSE == m->success) {
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);
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
}

void starotGaiaDialogStartTransport(GAIA_STAROT_IND_T *message) {
    if (appGetGaia()->transformAudioFlag == TRANSFORM_COMING) {
        gaiaStarotPrivateData.testSpeedIndex = 0;
        appGetGaia()->transformAudioFlag = DIALOG_CAN_TRANSFORM;
        appGetGaia()->nowSendAudioPhase = GAIA_TRANSFORM_AUDIO_IDLE;
        gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_ACCEPT_RECORD);
        DEBUG_LOG("call disable_audio_forward(FALSE);");
        gaiaStarotPrivateData.audioTransType = 0;
        gaiaClearDropAudioSize();
    }
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((appGetGaia()->transformAudioFlag & DIALOG_CAN_TRANSFORM) > 0 ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE),
                        0, NULL);
}

void starotGaiaDialogStopTransport(GAIA_STAROT_IND_T *message) {
    appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                        ((appGetGaia()->transformAudioFlag == DIALOG_CAN_TRANSFORM) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE), 0, NULL);
    if (appGetGaia()->transformAudioFlag == DIALOG_CAN_TRANSFORM) {
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

