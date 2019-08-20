#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>
#include <vmal.h>

#include "../av_headset.h"
//#include "gaia/gaia.h"
#include "../av_headset_gaia_starot.h"
#include "../av_headset_kymera_private.h"
#include "cap_id_prim.h"
#include "audio_forward.h"

#ifdef GAIA_TEST
extern uint16 bufferSendUnit;
#endif

#define AUDIO_CORE_0             (0)
#define AUDIO_DATA_FORMAT_16_BIT (0)

static void msg_handler (Task appTask, MessageId id, Message msg);
static void initSetSpeechDataSource(Source src);
static void sendMessageMoreData(Task task, Source src, uint32 delay);
static void indicateFwdDataSource(Source src, source_type_t type);
static void sendDataMessage(Source source);

static TaskData audioForwardTaskData = {.handler = msg_handler};
static Task audioForwardTask = &audioForwardTaskData;

Source globalSource;

static Source data_source_sco = NULL;
static Source data_source_mic = NULL;

void forwardAudioAndMic(kymera_chain_handle_t sco_chain)
{
    uint16 set_data_format[] = { OPMSG_PASSTHROUGH_ID_CHANGE_OUTPUT_DATA_TYPE, AUDIO_DATA_FORMAT_16_BIT };

    /** forward sco data  **/

    /* 1. load passthrough cap */
    Operator scofwd_passthrough = PanicZero(ChainGetOperatorByRole(sco_chain, OPR_CUSTOM_SCO_PASSTHROUGH));

    /* 2. set passthrough data format */
    PanicZero(VmalOperatorMessage(scofwd_passthrough, set_data_format,
                                  sizeof(set_data_format)/sizeof(set_data_format[0]), NULL, 0));

    /* 3. create timestamped endpoint from passthrough */
    Source scofwd_capture = StreamSourceFromOperatorTerminal(scofwd_passthrough, 0);
    PanicFalse(SourceMapInit(scofwd_capture, STREAM_TIMESTAMPED, 9));

    /* 4. setup MORE_DATA message. */
    indicateFwdDataSource(scofwd_capture, STYPE_SCO);

    /** forward mic data  **/

    /* 1. load passthrough cap */
    Operator micfwd_passthrough = PanicZero(ChainGetOperatorByRole(sco_chain, OPR_CUSTOM_MIC_PASSTHROUGH));

    /* 2. set passthrough data format */
    PanicZero(VmalOperatorMessage(micfwd_passthrough, set_data_format,
                                  sizeof(set_data_format)/sizeof(set_data_format[0]), NULL, 0));

    /* 3. create timestamped endpoint from passthrough */
    Source micfwd_capture = StreamSourceFromOperatorTerminal(micfwd_passthrough, 0);
    PanicFalse(SourceMapInit(micfwd_capture, STREAM_TIMESTAMPED, 9));

    /* 4. setup MORE_DATA message. */
    indicateFwdDataSource(micfwd_capture, STYPE_MIC);
}

void disconnectAudioForward(kymera_chain_handle_t sco_chain) {
    Source sco_audfwd_src = ChainGetOutput(sco_chain, EPR_AUDIO_SCOFWD_OUT);
    Source mic_audfwd_src = ChainGetOutput(sco_chain, EPR_AUDIO_MICFWD_OUT);

    /* Disconnect audio forward source */
    SourceUnmap(sco_audfwd_src);
    SourceUnmap(mic_audfwd_src);
}

unsigned nowSendStatus = 0;
int dissNum = 0;

void sendDataMessage(Source source) {
    int size = SourceSize(source);

    const uint16 *ptr = (const uint16*)SourceMap(source);

#ifdef GAIA_TEST
    if (NULL == appGetGaia()->transport) {
        SourceDrop(source, size);
    } else if ((nowSendStatus == 0) & (size >= (bufferSendUnit * 2))) {
        //printf("call xxx audio forward msg_handler GAIA_STAROT_COMMAND_IND for send nowSendStatus is %d, size is :%d\n",nowSendStatus, size);
        GAIA_STAROT_AUDIO_IND_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_IND_T));
        starot->command = GAIA_COMMAND_STAROT_CALL_AUDIO_IND;
        starot->source = source;
        starot->pos = (uint8*)ptr;
        starot->len = size;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
        nowSendStatus = 1;
    }
#else
    UNUSED(ptr);
    SourceDrop(source, size);
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
        Source source = message->source;
        int size = SourceSize(source);
        const uint16 *ptr = (const uint16*)SourceMap(source);

        if (source == data_source_sco)
        {
#if 0
            static unsigned more_loops_sco = 0, data_cnt_sco = 0;

            data_cnt_sco += size;

            if(0 == (++more_loops_sco) % 100)
                printf("sco: get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
                       more_loops_sco, size, data_cnt_sco, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);
#endif
            globalSource = message->source;
            sendDataMessage(message->source);
        }
        else if (source == data_source_mic)
        {
#if 0
            static unsigned more_loops_mic = 0, data_cnt_mic = 0;

            data_cnt_mic += size;

            if(0 == (++more_loops_mic) % 100)
                printf("mic: get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
                       more_loops_mic, size, data_cnt_mic, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);
#endif
        }
        else
            printf("ERROR: msg_handler: MESSAGE_MORE_DATA: INCORRECT SOURCE!\n");

        SourceDrop(source, size);
        break;
    }

#ifdef GAIA_TEST
    case GAIA_STAROT_COMMAND_IND:
    {
        /// 确认发送的消息
//        printf("call xxx audio forward msg_handler GAIA_STAROT_COMMAND_IND \n");

        GAIA_STAROT_AUDIO_CFM_T* message = (GAIA_STAROT_AUDIO_CFM_T*)msg;
        nowSendStatus = 0;
        if (NULL != message && message->command == GAIA_COMMAND_STAROT_CALL_AUDIO_CFM) {
            int size = SourceSize(message->source);
            if (size > 3000) {
                if (message->len > 0) {
                    dissNum += (400 - message->len);
                }
                SourceDrop(message->source, 400);
            } else {
                if (message->len > 0) {
                    SourceDrop(message->source, message->len);
                }
            }
            sendDataMessage(message->source);
        } else if (NULL != message && message->command == GAIA_COMMAND_STAROT_CALL_AUDIO_END) {
            printf("diss audio bytes is : %d\n", dissNum);
            dissNum = 0;
            nowSendStatus = 0;
        }

        break;
    }
#endif
    case AUDIO_VA_INDICATE_DATA_SOURCE:
    {
        AUDIO_VA_INDICATE_DATA_SOURCE_T* ind = (AUDIO_VA_INDICATE_DATA_SOURCE_T*)msg;

        initSetSpeechDataSource(ind->data_src);

        break;
    }
    default:
        break;
    }
}

static void indicateFwdDataSource(Source src, source_type_t type)
{
    if (STYPE_SCO == type)
        data_source_sco = src;
    else if (STYPE_MIC == type)
        data_source_mic = src;
    else {
        printf("ERROR: indicateFwdDataSource: INCORRECT SOURCE TYPE!\n");
        return;
    }

    MAKE_AUDIO_MESSAGE( AUDIO_VA_INDICATE_DATA_SOURCE, message) ;
    message->data_src = src;
    MessageSend(audioForwardTask, AUDIO_VA_INDICATE_DATA_SOURCE, message);
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
        sendMessageMoreData(audioForwardTask, src, 0);
}

static void sendMessageMoreData(Task task, Source src, uint32 delay)
{
    MessageMoreData *message = PanicUnlessMalloc(sizeof(MessageMoreData));
    message->source = src;
    MessageCancelAll(task, MESSAGE_MORE_DATA);
    MessageSendLater(task, MESSAGE_MORE_DATA, message, delay);
}

Task getAudioForwardTask(void) {
    return audioForwardTask;
}
