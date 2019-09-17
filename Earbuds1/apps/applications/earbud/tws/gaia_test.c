#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>
#include <vmal.h>

#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "av_headset_kymera_private.h"
#include "cap_id_prim.h"
#include "audio_forward.h"
#include "gaia_test.h"

unsigned nowSendStatus = 0;
int dissNum = 0;
extern uint16 bufferSendUnit;

void gaiaStartNotify()
{
#ifdef GAIA_TEST
    if (NULL != appGetGaia()->transport) {
        GAIA_STAROT_IND_T* starot = PanicUnlessNew(GAIA_STAROT_IND_T);
        starot->command = GAIA_COMMAND_STAROT_CALL_BEGIN;
        starot->payloadLen = 0;
        appGetGaia()->nowSendCallAudio = 1;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
    }
#endif
}

void handle_starot_gaia_msg(MessageId id, Message msg,
                            Source* data_source_sco, Source* data_source_mic)
{
    switch (id) {
        case GAIA_STAROT_COMMAND_IND:
        {
            /// 确认发送的消息
            GAIA_STAROT_AUDIO_CFM_T* message = (GAIA_STAROT_AUDIO_CFM_T*)msg;
            nowSendStatus = 0;
             if (NULL != message && message->command == GAIA_COMMAND_STAROT_CALL_AUDIO_END) {
                printf("diss audio bytes is : %d\n", dissNum);
                dissNum = 0;
                *data_source_sco = NULL;
                *data_source_mic = NULL;
                nowSendStatus = 0;
            }

            break;
        }
    }
}

/// true：发送数据到GAIA的TASK
/// false：没有发送消息到GAIA的TASK
bool sendDataMessage(Source source, enum GAIA_AUDIO_TYPE type,
                     Source data_source_sco, Source data_source_mic) {
    int size = SourceSize(source);

    /// 丢弃过多的数据，防止数据过多，导致source不可以使用
    if (size > 5 * bufferSendUnit) {
        dissNum += 5 * bufferSendUnit;
        size -= 5 * bufferSendUnit;
        printf("drop size 5 * %d\n", bufferSendUnit);
        SourceDrop(source, 5 * bufferSendUnit);
    }

    if (NULL == appGetGaia()->transport || NULL == data_source_sco || NULL == data_source_mic) {
        DEBUG_LOG("sendDataMessage,type=%d Drop:%d no cnnect", type, size);
        SourceDrop(source, size);
    } else if (size >= bufferSendUnit) {
        GAIA_STAROT_AUDIO_IND_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_IND_T));
        starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_IND;
        starot->source = source;
        starot->audioType = type;
        starot->data = NULL;
        starot->len = size;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
        nowSendStatus = 1;
        return TRUE;
    }
    return FALSE;
}
