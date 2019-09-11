
#include "av_headset_log.h"
#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "audio_forward.h"

#ifdef GAIA_TEST


Source dialogSpeaker = NULL;
Source dialogMic = NULL;

Source audioForwardSource = NULL;

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

uint16 bufferSendUnit = 40;
static int speakerDropNum = 0;
static int micDropNum = 0;

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message) {

    switch (message->command) {
        case GAIA_COMMAND_STAROT_FIRST_COMMAND: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_FIRST_COMMAND");
        }
            break;

        case GAIA_COMMAND_STAROT_START_SEND_TIMER: {
            appGaiaSendResponse(GAIA_VENDOR_STAROT, message->command, GAIA_STATUS_SUCCESS, 0, NULL);
            DEBUG_LOG("call GAIA_COMMAND_STAROT_START_SEND_TIMER");
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

        case GAIA_COMMAND_STAROT_CALL_BEGIN: {
            appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
            appGetGaia()->nowSendCallAudio = 1;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_CALL_BEGIN");
            appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_BEGIN, 0xfe, 0, NULL);
        }
            break;

        case GAIA_COMMAND_STAROT_CALL_END: {
            appGetGaia()->nowSendCallAudio = 0;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_CALL_END");
            DEBUG_LOG("call speakerDropNum :%d micDropNum:%d\n", speakerDropNum, micDropNum);
            appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);
            speakerDropNum = 0;
            micDropNum = 0;
            GAIA_STAROT_AUDIO_CFM_T *starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_CFM_T));
            starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_END;
            MessageSend(getAudioForwardTask(), GAIA_STAROT_COMMAND_IND, starot);
            audioForwardSource = NULL;
        }
            break;

        case GAIA_COMMAND_STAROT_CALL_AUDIO_IND:
            starotGaiaSendAudio(NULL);
            break;

        case GAIA_COMMAND_STAROT_SET_BOND_CODE_IND:
            if (!appBleIsBond()) {
                uint8 *data = message->payload;
                uint16 advCode = (((uint16) data[0]) << 8) | data[1];
                uint32 bindCode = (((uint32) data[2]) << 24) |(((uint32) data[3]) << 16) | (((uint32) data[4]) << 8) | data[5];
                appBleSetPond(advCode, bindCode);
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
                    uint32 bindCode = (((uint32) data[0]) << 24) |(((uint32) data[1]) << 16) | (((uint32) data[2]) << 8) | data[3];
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

/*
 * 使用事件去驱动让audio_forward去发送数据
 */
void starotGaiaParseMessageMoreSpace(void) {
    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }

    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
    /// 尝试使用messagemorespace这唯一的命令去让他发送消息
    starotNotifyAudioForward(FALSE, 0);
}

void starotNotifyAudioForward(bool st, uint8 flag) {
    UNUSED(st);
    if (appGetGaia()->nowSendCallAudio <= 0) {
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

    starotGaiaSendAudio(NULL);
}

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T *message) {
    UNUSED(message);

    if (appGetGaia()->nowSendCallAudio <= 0) {
        return FALSE;
    }

    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }
    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;

    static uint8 payload[512];
    uint8 flag = 0;
    uint16 pos = 1;

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

    payload[0] = flag;

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

#endif
