#include <Panic.h>
#include <stdlib.h>
#include <stdio.h>
#include <message.h>
#include <source.h>
#include <operator.h>
#include <operators.h>
#include <vmal.h>
#include <cap_id_prim.h>
#include "pio_common.h"
#include "audio_plugin_if.h"
#include "audio.h"
#include "packetiser_helper.h"
#include "opmsg_prim.h"

#include "av_headset.h"

#ifdef CONFIG_LIS25BA_TEST


#define MESSAGE_LIS25BA_START       (0x8200)
#define MESSAGE_LIS25BA_STOP        (0x8201)

// #define SYNC_CHANNEL_INPUT             // 多通道数据同步，3入1出
#define LIS25_CHANNELS        (1)        // 最多三路音频

typedef struct tagLIS25TEST {
    TaskData         task;

    BundleID         bID;
    Operator         opPassthrough;
    Source           sourceLis[LIS25_CHANNELS];
    Source           sourceMap[LIS25_CHANNELS];

    unsigned         data_loops, data_cnt;  // 接收数据的次数及部长度
    uint8            iStared;               // 已经运行，不能多次启动
}LisTest, *LisTestPtr;
static LisTest theLis25Test;


static void InitSetSpeechDataSource(Task pTask)
{
    int i;
    LisTestPtr pLis25 = (LisTestPtr)pTask;

    for(i = 0; i < LIS25_CHANNELS; i++) {
        if(pLis25->sourceMap[i]) {
            MessageStreamTaskFromSource(pLis25->sourceMap[i], pTask);
            SourceConfigure(pLis25->sourceMap[i], VM_SOURCE_MESSAGES, VM_MESSAGES_ALL);
        }
    }
}

static void lis25InitStartStream(Task pTask);
static void lis25InitStopStream(Task pTask);
static void lis25test_handler (Task pTask, MessageId id, Message msg)
{
    LisTestPtr pLis25 = (LisTestPtr)pTask;

    switch (id) {
    case MESSAGE_MORE_DATA:{
           MessageMoreData * message = (MessageMoreData *)msg;
           int size = SourceSize(message->source);
           const uint16 *ptr = (const uint16*)SourceMap(message->source);

           pLis25->data_cnt += size;
           if(pLis25->data_loops % 333 == 0)
                printf("get=%d data=%d cnt=%d [%04x %04x   %04x %04x %04x %04x\n", pLis25->data_loops, size, pLis25->data_cnt,
                       ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);

           pLis25->data_loops += 1;
           SourceDrop(message->source, size);
        }
        break;

    case AUDIO_VA_INDICATE_DATA_SOURCE:
        InitSetSpeechDataSource(pTask);
        break;

    case MESSAGE_LIS25BA_START:
        if(pLis25->iStared) {
            printf("Please Stop it First");
            return;
        }
        lis25InitStartStream(&pLis25->task);
        pLis25->iStared = 1;
        break;
    case MESSAGE_LIS25BA_STOP:
        if(0 == pLis25->iStared)
            return;
        lis25InitStopStream(&pLis25->task);
        pLis25->iStared = 0;
        break;
    default:
        printf("default id=%d== \n", id);
        break;
    }
}
static void indicateDataSource(Task pTask)
{
    LisTestPtr pLis25 = (LisTestPtr)pTask;
    MAKE_AUDIO_MESSAGE( AUDIO_VA_INDICATE_DATA_SOURCE, message) ;
    message->data_src = pLis25->sourceMap[0];
    MessageSend(pTask, AUDIO_VA_INDICATE_DATA_SOURCE, message);
}


static void lis25InitStartStream(Task pTask)
{
    signed i;
    LisTestPtr pLis25 = (LisTestPtr)pTask;
    FILE_INDEX index=FILE_NONE;
    const char download_passthrough[] = "download_passthrough.dkcs";

    OperatorFrameworkEnable(1);

    for(i = 0; i < LIS25_CHANNELS; i++) {
        pLis25->sourceLis[i] = PanicNull(StreamAudioSource(AUDIO_HARDWARE_PCM, AUDIO_INSTANCE_0, AUDIO_CHANNEL_SLOT_0+i));
        SourceI2SConfigure(pLis25->sourceLis[i]);
#ifdef SYNC_CHANNEL_INPUT
        if(i > 0)
            PanicFalse(SourceSynchronise(pLis25->sourceLis[i-1], pLis25->sourceLis[i]));
#endif
    }
    SourceSinkI2SMasterClockEnable(pLis25->sourceLis[0], 1, 1);    // Enable Clock

    index = FileFind(FILE_ROOT, download_passthrough, strlen(download_passthrough));
    if(index != FILE_NONE)   {
        pLis25->bID = PanicZero(OperatorBundleLoad(index, 0));
       pLis25->opPassthrough = PanicZero(VmalOperatorCreate(CAP_ID_DOWNLOAD_PASSTHROUGH));
  //       pLis25->opPassthrough = PanicZero(VmalOperatorCreate(CAP_ID_DOWNLOAD_TTP_PASS));
    }
    else
        Panic();

    // Set DataFormat
    uint16 set_data_format[] = { OPMSG_PASSTHROUGH_ID_CHANGE_OUTPUT_DATA_TYPE, operator_data_format_pcm };
    PanicZero(VmalOperatorMessage(pLis25->opPassthrough, set_data_format, sizeof(set_data_format)/sizeof(set_data_format[0]), NULL, 0));
//    OperatorsStandardSetBufferSizeWithFormat(pLis25->opPassthrough, 4090, operator_data_format_pcm);

    for(i = 0; i < LIS25_CHANNELS; i++) {
         /* And connect everything  ...line_in to the passthrough operator */
         PanicNull(StreamConnect(pLis25->sourceLis[i], StreamSinkFromOperatorTerminal(pLis25->opPassthrough, i)));

      #ifndef SYNC_CHANNEL_INPUT
         pLis25->sourceMap[i] = StreamSourceFromOperatorTerminal(pLis25->opPassthrough, i);
         PanicFalse(SourceMapInit(pLis25->sourceMap[i], STREAM_TIMESTAMPED, AUDIO_FRAME_METADATA_LENGTH));
      #endif
    }
#ifdef SYNC_CHANNEL_INPUT
    pLis25->sourceMap[0] = StreamSourceFromOperatorTerminal(pLis25->opPassthrough, 0);
    PanicFalse(SourceMapInit(pLis25->sourceMap[0], STREAM_TIMESTAMPED, AUDIO_FRAME_METADATA_LENGTH));
#endif

    indicateDataSource(pTask);

    PanicFalse(OperatorStartMultiple(1, &pLis25->opPassthrough, NULL));
}

static void lis25InitStopStream(Task pTask)
{
    uint16 i;
    LisTestPtr pLis25 = (LisTestPtr)pTask;

    PanicFalse(OperatorStopMultiple(1, &pLis25->opPassthrough, NULL));

    for(i = 0; i < LIS25_CHANNELS; i++) {
        StreamDisconnect(pLis25->sourceLis[i], StreamSinkFromOperatorTerminal(pLis25->opPassthrough, i));

        if(pLis25->sourceMap[i]) {
            SourceUnmap(pLis25->sourceMap[i]);
            pLis25->sourceMap[i] = NULL;
        }
        SourceClose(pLis25->sourceLis[i]), pLis25->sourceLis[i] = NULL;
    }

    PanicFalse(OperatorDestroyMultiple(1, &pLis25->opPassthrough, NULL));
    PanicFalse(OperatorBundleUnload(pLis25->bID));
}

void lis25Test(void)
{
    memset(&theLis25Test, 0, sizeof(theLis25Test));
    theLis25Test.task.handler  = lis25test_handler;
}

int lis25TestPower(bool isOn)
{
    if(isOn) {
        if(theLis25Test.iStared) {
            printf("Please Stop it First");
            return -1;
        }
        MessageSend(&theLis25Test.task, MESSAGE_LIS25BA_START, 0);
    }
    else {
        if(0 == theLis25Test.iStared)
            return -1;

        MessageSend(&theLis25Test.task, MESSAGE_LIS25BA_STOP, 0);
    }
    return 0;
}

#else
void lis25Test(void) { printf("Please CONFIG_LIS25BA_TEST macro");}
int lis25TestPower(bool isOn) {(void)isOn; return 0;}
#endif
