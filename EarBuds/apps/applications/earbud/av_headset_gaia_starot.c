#ifdef GAIA_TEST

#include "av_headset_gaia_starot.h"
#include "av_headset_log.h"
#include "av_headset.h"
#include "pmalloc.h"

extern Task audioForwardTask;
Source audioForwardSource = NULL;

extern void appGaiaSendResponse(uint16 vendor_id, uint16 command_id, uint16 status,
                                uint16 payload_length, uint8 *payload);

extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);

//static bool starotAudioBufferWriteEx(uint16 len, uint8 *payload);

bool checkWriteInRange(int begin, int end, int pos);

bool checkReadInRange(int begin, int end, int pos);


//static const uint16 bufferSize = 1024 * 1;
//
//static uint16 bufferReadPos = 0;
//static uint16 bufferWritePos = 0;
uint16 bufferSendUnit = 240;
//static uint16 bufferTempReadPos = 0;
//static uint8 buffer[1024 * 1];

bool starotAudioBufferInit(void) {
//    bufferReadPos = 0;
//    bufferWritePos = 0;
    return TRUE;
}

//bool starotAudioBufferWriteEx(uint16 len, uint8 *payload) {
//    UNUSED(len);
//    UNUSED(payload);
////    memcpy(buffer + bufferWritePos, payload, len);
////    bufferWritePos = (bufferWritePos + len) % bufferSize;
//    return TRUE;
//}

bool checkWriteInRange(int begin, int end, int pos) {
    if (begin < pos && pos <= end) {
        return TRUE;
    }
    return FALSE;
}

bool checkReadInRange(int begin, int end, int pos) {
    if (begin <= pos && pos < end) {
        return TRUE;
    }
    return FALSE;
}

bool starotAudioBufferWrite(uint16 len, uint8 *payload) {
    UNUSED(len);
    UNUSED(payload);
//    if ((bufferWritePos + len) > bufferSize) {
//        if (TRUE == checkWriteInRange(bufferWritePos, bufferSize, bufferReadPos) ||
//            TRUE == checkWriteInRange(0, (bufferWritePos + len) % bufferSize, bufferReadPos)) {
//            DEBUG_LOG("starotAudioBufferWrite no space write pos :%04x, read pos :%04x, len:%04x",
//                      bufferWritePos, bufferReadPos, len);
//            return FALSE;
//        }
//    } else {
//        if (TRUE == checkWriteInRange(bufferWritePos, bufferWritePos + len, bufferReadPos)) {
//            DEBUG_LOG("starotAudioBufferWrite no space write pos :%04x, read pos :%04x, len:%04x",
//                      bufferWritePos, bufferReadPos, len);
//            return FALSE;
//        }
//    }
//
//    if ((bufferWritePos + len) > bufferSize) {
//        int s1 = bufferSize - bufferWritePos;
//        int s2 = len - (bufferSize - bufferWritePos);
//        starotAudioBufferWriteEx(s1, payload);
//        starotAudioBufferWriteEx(s2, payload + s1);
//    } else {
//        starotAudioBufferWriteEx(len, payload);
//    }
    return TRUE;
}

bool starotGaiaResend(GAIA_STAROT_IND_T *message) {
    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe,
                                message->payloadLen, message->payload);
    return st;
}

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
            //                static const lp_power_table lp_powertable_data_access[]=
            //                {
            //                    /* mode,        min_interval,   max_interval,   attempt,    timeout,    duration */
            //                    {lp_active,    0,              0,              0,          0,          0}      /* Go into active mode and stay there */
            //                };
            //                ConnectionSetLinkPolicy(theSppServiceData.sink, 1 ,lp_powertable_data_access);
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
            MessageSend(audioForwardTask, GAIA_STAROT_COMMAND_IND, starot);
            audioForwardSource = NULL;
        }
            break;

        case GAIA_COMMAND_STAROT_CALL_AUDIO_IND:
            starotGaiaSendAudio((GAIA_STAROT_AUDIO_IND_T*)message);
            break;
    }
    return TRUE;
}

//bool starotGaiaSendAudio(void) {
//    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
//        return FALSE;
//    }
//
//    if ((bufferReadPos + bufferSendUnit) > bufferSize) {
//        if (TRUE == checkReadInRange(bufferReadPos, bufferSize, bufferWritePos) ||
//            TRUE == checkReadInRange(0, (bufferReadPos + bufferSendUnit) % bufferSize, bufferWritePos)) {
//            DEBUG_LOG("starotGaiaSendAudio 1 no data send pos :%04x, read pos :%04x, len:%04x",
//                      bufferWritePos, bufferReadPos, bufferSendUnit);
//            return FALSE;
//        }
//    } else {
//        if (TRUE == checkReadInRange(bufferReadPos, bufferReadPos + bufferSendUnit, bufferWritePos)) {
//            DEBUG_LOG("starotGaiaSendAudio 2 no data send pos :%04x, read pos :%04x, len:%04x",
//                      bufferWritePos, bufferReadPos, bufferSendUnit);
//            return FALSE;
//        }
//    }
//
//    static uint8 payload[201];
//    payload[0] = 1;
//    bufferTempReadPos = bufferReadPos;
//
//    if ((bufferTempReadPos + bufferSendUnit) > bufferSize) {
//        uint16 s1 = bufferSize - bufferTempReadPos;
//        uint16 s2 = bufferSendUnit - s1;
//
//        memcpy(payload + 1, buffer + bufferTempReadPos, s1);
//        bufferTempReadPos = (bufferTempReadPos + s1) % bufferSize;
//
//        memcpy(payload + 1 + s1, buffer + bufferTempReadPos, s2);
//        bufferTempReadPos = (bufferTempReadPos + s2) % bufferSize;
//    } else {
//        memcpy(payload + 1, buffer + bufferTempReadPos, bufferSendUnit);
//        bufferTempReadPos = (bufferTempReadPos + bufferSendUnit) % bufferSize;
//    }
//
//    bool st = appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_CALL_AUDIO_IND, 0xfe,
//                                bufferSendUnit + 1, payload);
//
//    if (TRUE == st) {
//        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_ING;
//    } else {
//        appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE;
//        DEBUG_LOG("send data failed");
//        //// 分配内存失败，需要使用定时器/数据驱动
//    }
//
//    return TRUE;
//}



void starotSwapBufferReadPos(void) {
//    bufferReadPos = bufferTempReadPos;
}

//void starotGaiaParseMessageMoreSpace(void) {
//    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
//        return;
//    }
//    appGetGaia()->nowSendAudio = GAIA_TRANSFORM_AUDIO_IDLE;
//    starotGaiaSendAudio();
//}

/*
 * 使用事件去驱动让audio_forward去发送数据
 */
void starotGaiaParseMessageMoreSpace(void) {

    /// todo 通知audio_forward继续发送
    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE) {
        return;
    }
//    printf("call xxx starotGaiaParseMessageMoreSpace \n");

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
    starot->pos = 0;
    starot->len = (TRUE == st ? bufferSendUnit * 2 : 0);
    MessageSend(audioForwardTask, GAIA_STAROT_COMMAND_IND, starot);
    audioForwardSource = NULL;
}

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T* message) {
    if (appGetGaia()->nowSendCallAudio <= 0) {
        return FALSE;
    }
//    printf("call xxx starotGaiaSendAudio \n");

    if (appGetGaia()->nowSendAudio != GAIA_TRANSFORM_AUDIO_IDLE) {
        return FALSE;
    }
    static uint8 payload[500];
    payload[0] = 1;
    audioForwardSource = message->source;

    for (int i = 0; i < bufferSendUnit; i += 2) {
        payload[1 + i] = (message->pos)[i * 2 + 1];
        payload[1 + i + 1] = (message->pos)[i * 2];
        /// 如果播放有问题，就将值交换一下
    }
//    memcpy(payload + 1, message->pos, bufferSendUnit);
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
