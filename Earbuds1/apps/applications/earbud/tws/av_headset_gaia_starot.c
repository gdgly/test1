#include <pmalloc.h>

#include "av_headset_log.h"
#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "audio_forward.h"
#include "tws/attr.h"
#include "tws/audio_forward.h"

uint16 bufferSendUnit = 40;

#ifdef GAIA_TEST

Source dialogSpeaker = NULL;
Source dialogMic = NULL;

Source audioForwardSource = NULL;
int audioTransType = 0;
uint16 speedTestSendUnit = 81;

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

static void gaiaParseDialogStatus(GAIA_STAROT_IND_T *message);

static void gaiaNotifyAudioAcceptStatus(Task task, int command);

static int speakerDropNum = 0;
static int micDropNum = 0;
extern uint8 testSpeedIndex;

static void starotSpeedSendIntervalParse(void);

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message) {

    switch (message->command) {
        case GAIA_COMMAND_STAROT_FIRST_COMMAND: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_FIRST_COMMAND");
        }
            break;

        case GAIA_COMMAND_STAROT_START_SEND_TIMER: {
            testSpeedIndex = 0;
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            if (message->payloadLen > 0) {
                speedTestSendUnit = message->payload[0] + 1;
            } else {
                speedTestSendUnit = 80 + 1;
            }
            appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_START_SEND_TIMER, unit is : %d", speedTestSendUnit);
            appGetGaia()->needCycleSendAudio = 1;
            MessageSendLater(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL, 10);
        }
            break;

        case GAIA_COMMAND_STAROT_STOP_SEND_TIMER: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_STOP_SEND_TIMER");
            appGetGaia()->needCycleSendAudio = 0;
        }
            break;

        case STAROT_DIALOG_STATUS:
            gaiaParseDialogStatus(message);
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

        case STAROT_DIALOG_AUDIO_DATA:
            starotGaiaSendAudio(NULL);
            break;

            /// APP希望接受耳机的音频
        case GAIA_COMMAND_STAROT_START_TRANS_AUDIO_IND:
            if (appGetGaia()->nowSendCallAudio == DIALOG_COMING) {
                appGetGaia()->nowSendCallAudio |= DIALOG_CAN_TRANSFORM;
                gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_ACCEPT_RECORD);
                disable_audio_forward(FALSE);
                audioTransType = 0;
            }
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                                ((appGetGaia()->nowSendCallAudio & DIALOG_CAN_TRANSFORM) > 0 ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE),
                                0, NULL);
            break;

            /// APP希望停止接受音频或压根从头不想要音频数据
        case GAIA_COMMAND_STAROT_END_TRANS_AUDIO_IND:
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command,
                                ((appGetGaia()->nowSendCallAudio >= DIALOG_COMING) ? GAIA_STATUS_SUCCESS : GAIA_STATUS_INCORRECT_STATE), 0, NULL);
            if (appGetGaia()->nowSendCallAudio > DIALOG_NONE) {
                gaiaNotifyAudioAcceptStatus(appGetUiTask(), STAROT_DIALOG_USER_REJECT_RECORD);
                disable_audio_forward(TRUE);
                appGetGaia()->nowSendCallAudio = DIALOG_NONE;
            }
            break;

        case GAIA_COMMAND_STAROT_SET_BOND_CODE_IND:
            if (!appBleIsBond()) {
                uint8 *data = message->payload;
                uint16 advCode = (((uint16) data[0]) << 8) | data[1];
                uint32 bindCode = (((uint32) data[2]) << 24) | (((uint32) data[3]) << 16) | (((uint32) data[4]) << 8) | data[5];
                appBleSetBond(advCode, bindCode);
                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
                /// 取消超时如果没有发送bondCode断开连接的定时器
            } else {
                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
            }
            break;

        case GAIA_COMMAND_STAROT_CHECK_BOND_CODE_IND:
            do {
                if (appBleIsBond()) {
                    uint8 *data = message->payload;
                    uint32 bindCode = (((uint32) data[0]) << 24) | (((uint32) data[1]) << 16) | (((uint32) data[2]) << 8) | data[3];
                    if (bindCode == appBleGetBondCode()) {
                        appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
                        /// 取消超时如果没有发送bondCode断开连接的定时器
                        break;
                    }
                }

                appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_NOT_SUPPORTED, 0, NULL);
            } while (0);

            break;
    }
    return TRUE;
}

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
            if (DIALOG_NONE != appGetGaia()->nowSendCallAudio) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_BEGIN_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
            break;

        case STAROT_DIALOG_CALL_END_TIMEOUT:
            if (DIALOG_NONE == appGetGaia()->nowSendCallAudio) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_END_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
            break;

        case STAROT_DIALOG_CALL_ATTR_TIMEOUT:
            if (DIALOG_NONE != appGetGaia()->nowSendCallAudio) {
                DEBUG_LOG("recv STAROT_DIALOG_CALL_ATTR_TIMEOUT, need retry send to app");
                StarotResendCommand *cmd = starotResendCommandDo((StarotResendCommand *) message, TRUE);
                MessageSendLater(appGetGaiaTask(), id, cmd, STAROT_COMMAND_TIMEOUT);
            }
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
    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }

    if (appGetGaia()->needCycleSendAudio > 0) {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
        MessageSend(&appGetGaia()->gaia_task, GAIA_STAROT_AUDIO_INTERVAL, NULL);
    } else {
        /// 尝试使用messagemorespace这唯一的命令去让他发送消息
        starotNotifyAudioForward(FALSE, 0);
    }
}

void starotNotifyAudioForward(bool st, uint8 flag) {
    UNUSED(st);
    if (appGetGaia()->nowSendCallAudio <= DIALOG_NONE) {
        return;
    }

    if (TRUE == st && flag > 0) {
        if ((flag & GAIA_AUDIO_SPEAKER) > 0 && NULL != dialogSpeaker) {
            speakerDropNum += bufferSendUnit;
            SourceDrop(dialogSpeaker, bufferSendUnit);
        }
        if ((flag & GAIA_AUDIO_MIC) > 0 && NULL != dialogMic) {
            micDropNum += bufferSendUnit;
            SourceDrop(dialogMic, bufferSendUnit);
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
    if (appGetGaia()->nowSendCallAudio < DIALOG_CAN_TRANSFORM) {
        return FALSE;
    }

    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }
    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;

    static uint8 payload[512];
    uint16 flag = 0, pos = 1;

    if (NULL != dialogSpeaker) {
        int size = SourceSize(dialogSpeaker);
        if (size >= bufferSendUnit) {
            flag |= GAIA_AUDIO_SPEAKER;
            const uint8 *ptr = (const uint8 *) SourceMap(dialogSpeaker);
            memcpy(payload + pos, ptr, bufferSendUnit);
            pos += bufferSendUnit;
        }
    }

    if (NULL != dialogMic) {
        int size = SourceSize(dialogMic);
        if (size >= bufferSendUnit) {
            flag |= GAIA_AUDIO_MIC;
            const uint8 *ptr = SourceMap(dialogMic);
            memcpy(payload + pos, ptr, bufferSendUnit);
            pos += bufferSendUnit;
        }
    }

    payload[0] = (uint8) flag;
    audioTransType = flag;

    if (flag <= 0) {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
        return FALSE;
    }

    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe, pos, payload);
    if (TRUE == st) {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;
    } else {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
        //// 分配内存失败，需要使用定时器/数据驱动
        DEBUG_LOG("send data failed, wait more memory");
    }

    return TRUE;
}

void notifyGaiaDialogSource(Source speaker, Source mic) {
    dialogSpeaker = speaker;
    dialogMic = mic;
}

uint8 starotGaiaTransGetAudioType(void) {
    return audioTransType;
}

void gaiaParseDialogStatus(GAIA_STAROT_IND_T *message) {
    uint8 status = message->payload[0];
    uint8 dialogIn = 0X01, dialogOut = 0X02, dialogActive = 0X04, dialogInActive = 0X08;

    DEBUG_LOG("call STAROT_DIALOG_STATUS STATUS IS %02x", status);
    StarotAttr *head = NULL;

    /// 电话接入
    if ((((status & dialogIn) > 0) && ((appGetGaia()->status & dialogIn) < 1))
        || (((status & dialogOut) > 0) && ((appGetGaia()->status & dialogOut) < 1))) {
        DEBUG_LOG("Send GAIA_COMMAND_STAROT_CALL_BEGIN");
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_BEGIN, 0xfe, 0, NULL);
        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_BEGIN, 0, 0);
        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_BEGIN_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);

        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X02;
        attr->payload[0] = ((status & dialogOut) > 0) ? 0X02 : 0X01;

        appGetGaia()->nowSendCallAudio = DIALOG_COMING;
        appGetGaia()->status = appGetGaia()->status | dialogIn | dialogOut;
    }

    /// 电话接通
    if ((status & dialogActive) > 0) {
        StarotAttr *attr = attrMalloc(&head, 1);
        attr->attr = 0X05;
        attr->payload[0] = 0x02;
    }

    /// 电话挂断
    if ((0 == status) || (status & dialogInActive) > 0) {
        DEBUG_LOG("Send GAIA_COMMAND_STAROT_CALL_END");
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);
        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_END, 0, 0);
        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_END_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);

        appGetGaia()->status = 0;
        appGetGaia()->nowSendCallAudio = DIALOG_NONE;
    }

    /// 电话号码
    if (message->payloadLen > 2) {
        uint8 *phone = message->payload + 2;
        int len = message->payloadLen - 2;
        StarotAttr *attr = attrMalloc(&head, len);
        attr->attr = 0X01;
        memcpy(attr->payload, phone, len);
    }

    /// 发送通话属性
    if (NULL != head) {
        uint16 len = 0;
        uint8 *data = attrEncode(head, &len);
        DEBUG_LOG("len is :%d %p", len, data);
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_ATTR, 0xfe, len, data);
        StarotResendCommand *resend = starotResendCommandInit(GAIA_COMMAND_STAROT_CALL_ATTR, len, data);
        MessageSendLater(appGetGaiaTask(), STAROT_DIALOG_CALL_ATTR_TIMEOUT, resend, STAROT_COMMAND_TIMEOUT);
        attrFree(head, data);
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

//    DEBUG_LOG("call starotSpeedSendIntervalParse, nowSendAudio: %d", appGetGaia()->nowSendAudio);
    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
        return;
    }

    static uint8 data[201];
    for (int i = 1; i <= 1; ++i) {
        data[0] = i;
        data[1] = testSpeedIndex;
        appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe, speedTestSendUnit, data);
//        DEBUG_LOG("now send speed index is %02x", testSpeedIndex);
    }
    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;

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

#endif
