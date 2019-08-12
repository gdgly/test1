#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>

#include "../av_headset.h"
//#include "gaia/gaia.h"
#include "../av_headset_gaia_starot.h"

#include "audio_forward.h"



static void msg_handler (Task appTask, MessageId id, Message msg);
static void initSetSpeechDataSource(Source src);
static void sendMessageMoreData(Task task, Source src, uint32 delay);

static TaskData audioForwardTaskData = {.handler = msg_handler};
Task audioForwardTask = &audioForwardTaskData;

static void sendDataMessage(Source source);

void indicateFwdDataSource(Source src)
{
    MAKE_AUDIO_MESSAGE( AUDIO_VA_INDICATE_DATA_SOURCE, message) ;
    message->data_src = src;
    MessageSend(audioForwardTask, AUDIO_VA_INDICATE_DATA_SOURCE, message);
}

unsigned more_loops = 0, data_cnt = 0;
unsigned nowSendStatus = 0;

void sendDataMessage(Source source) {
    int size = SourceSize(source);
    const uint16 *ptr = (const uint16*)SourceMap(source);

    data_cnt += size;

    if(0 == (++more_loops) % 100)
        printf("get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
               more_loops, size, data_cnt, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);

#ifdef GAIA_TEST
    printf("call xxx audio forward msg_handler GAIA_STAROT_COMMAND_IND for send nowSendStatus is %d, size is :%d\n",nowSendStatus, size);

    if ((nowSendStatus == 0) & (size > 200)) {

        GAIA_STAROT_AUDIO_IND_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_IND_T));
        starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_IND;
        starot->source = source;
        starot->pos = (uint8*)ptr;
        starot->len = size;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
        nowSendStatus = 1;
    }
#else
//    SourceDrop(source, size);
    SourceDrop(source, 200);

#endif
//    printf("call xxx audio forward msg_handler indicateFwdDataSource \n");

}


static void msg_handler (Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    switch (id) {
    case MESSAGE_MORE_DATA:
    {
        MessageMoreData * message = (MessageMoreData *)msg;
        sendDataMessage(message->source);
        break;
    }
    case GAIA_STAROT_COMMAND_IND:
    {
        /// 确认发送的消息
        printf("call xxx audio forward msg_handler GAIA_STAROT_COMMAND_IND \n");

        GAIA_STAROT_AUDIO_CFM_T* message = (GAIA_STAROT_AUDIO_CFM_T*)msg;
        nowSendStatus = 0;
        if (NULL != message && message->len > 0 &&
            message->command == GAIA_COMMAND_STAROT_CALL_AUDIO_CFM) {

            SourceDrop(message->source, message->len * 3);
            sendDataMessage(message->source);
        }

        break;
    }

    case AUDIO_VA_INDICATE_DATA_SOURCE:
    {
        AUDIO_VA_INDICATE_DATA_SOURCE_T* ind = (AUDIO_VA_INDICATE_DATA_SOURCE_T*)msg;

        initSetSpeechDataSource(ind->data_src);

        break;
    }
    default:
        printf("default id=%d== \n", id);
        break;
    }
}

static void initSetSpeechDataSource(Source src)
{
    uint16 source_size;

    MessageStreamTaskFromSource(src, audioForwardTask);

    SourceConfigure(src, VM_SOURCE_MESSAGES, VM_MESSAGES_ALL);

    source_size = SourceSize(src);

    /* If this already contains data we may have missed the message
       so repost it*/
    if(source_size)
    {
        sendMessageMoreData(audioForwardTask, src, 0);
    }
}

static void sendMessageMoreData(Task task, Source src, uint32 delay)
{
    MessageMoreData *message = PanicUnlessMalloc(sizeof(MessageMoreData));
    message->source = src;
    MessageCancelAll(task, MESSAGE_MORE_DATA);
    MessageSendLater(task, MESSAGE_MORE_DATA, message, delay);

}
