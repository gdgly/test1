
#include "av_headset_gaia_starot.h"
#include "av_headset_log.h"
#include "av_headset.h"
#include "audio_route/audio_forward.h"

#ifdef GAIA_TEST

Source audioForwardSource = NULL;

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

uint16 bufferSendUnit = 150;


bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message) {
//    DEBUG_LOG("starotGaiaHandleCommand command: 0x%x", message->command);

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
//              starotAudioBufferInit();
                appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_BEGIN, 0xfe, 0, NULL);
            }
            break;

        case GAIA_COMMAND_STAROT_CALL_END: {
            appGetGaia()->nowSendCallAudio = 0;
            DEBUG_LOG("call GAIA_COMMAND_STAROT_CALL_END");
            appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_END, 0xfe, 0, NULL);

            GAIA_STAROT_AUDIO_CFM_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_CFM_T));
            starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_END;
            MessageSend(getAudioForwardTask(), GAIA_STAROT_COMMAND_IND, starot);
            audioForwardSource = NULL;
        }
            break;

        case GAIA_COMMAND_STAROT_CALL_AUDIO_IND:
            DEBUG_LOG("call GAIA_COMMAND_STAROT_CALL_AUDIO_IND");
            starotGaiaSendAudio((GAIA_STAROT_AUDIO_IND_T*)message);
            break;
    }
    return TRUE;
}

/*
 * 使用事件去驱动让audio_forward去发送数据
 */
void starotGaiaParseMessageMoreSpace(void) {

    /// todo 通知audio_forward继续发送
    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }
    printf("call xxx starotGaiaParseMessageMoreSpace \n");

    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
    /// 尝试使用messagemorespace这唯一的命令去让他发送消息
//    audioForwardSource = NULL;
    starotNotifyAudioForward(FALSE);
}

void starotNotifyAudioForward(bool st) {
    if (appGetGaia()->nowSendCallAudio <= 0) {
        return;
    }

//    printf("call xxx starotNotifyAudioForward \n");
    GAIA_STAROT_AUDIO_CFM_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_CFM_T));
    starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_CFM;
    starot->source = audioForwardSource;
    starot->data = 0;
    starot->len = (TRUE == st ? bufferSendUnit * 2 : 0);
    MessageSend(getAudioForwardTask(), GAIA_STAROT_COMMAND_IND, starot);
    audioForwardSource = NULL;
}

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T* message) {

    if (appGetGaia()->nowSendCallAudio <= 0) {
        return FALSE;
    }

    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }

    static uint8 payload[512];
    if (GAIA_AUDIO_SPEAKER == message->audioType) {
        payload[0] = 1;
    } else if (GAIA_AUDIO_MIC == message->audioType) {
        payload[0] = 2;
    }
    audioForwardSource = message->source;

    for (int i = 0; i < bufferSendUnit; i += 2) {
        payload[1 + i] = message->data[i * 2 + 1];
        payload[1 + i + 1] = message->data[i * 2];
    }

    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe,
                                bufferSendUnit + 1, payload);

    if (TRUE == st) {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;
    } else {
        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
        //// 分配内存失败，需要使用定时器/数据驱动
        DEBUG_LOG("send data failed, wait more memory");
    }

    return TRUE;
}

#endif
