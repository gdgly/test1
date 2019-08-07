#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>

#include "audio_forward.h"



static void msg_handler (Task appTask, MessageId id, Message msg);
static void initSetSpeechDataSource(Source src);
static void sendMessageMoreData(Task task, Source src, uint32 delay);

static TaskData audioForwardTaskData = {.handler = msg_handler};
static Task audioForwardTask = &audioForwardTaskData;

void indicateFwdDataSource(Source src)
{
    MAKE_AUDIO_MESSAGE( AUDIO_VA_INDICATE_DATA_SOURCE, message) ;
    message->data_src = src;
    MessageSend(audioForwardTask, AUDIO_VA_INDICATE_DATA_SOURCE, message);
}

unsigned more_loops = 0, data_cnt = 0;
static void msg_handler (Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    switch (id) {
    case MESSAGE_MORE_DATA:
    {
        MessageMoreData * message = (MessageMoreData *)msg;
        int size = SourceSize(message->source);
        const uint16 *ptr = (const uint16*)SourceMap(message->source);

        data_cnt += size;

        if(0 == (++more_loops) % 100)
            printf("get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
                   more_loops, size, data_cnt, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);

        SourceDrop(message->source, size);

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
