#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>
#include <vmal.h>

#include "public.h"
#include "av_headset.h"
#include "av_headset_gaia_starot.h"
#include "av_headset_kymera_private.h"
#include "cap_id_prim.h"
#include "audio_forward.h"
#include "gaia_test.h"

#define AUDIO_CORE_0             (0)
#define AUDIO_DATA_FORMAT_16_BIT (0)
#define OPMSG_PASSTHROUGH_ID_DISABLE_AUDIO_FORWARD (0x000C)

#ifdef GAIA_TEST
#define handle_sco_msg() do {sendDataMessage(message->source, GAIA_AUDIO_SPEAKER, data_source_sco, data_source_mic); }while(0)
#define handle_mic_msg() do {sendDataMessage(message->source, GAIA_AUDIO_MIC, data_source_sco, data_source_mic); }while(0)
#else
#define handle_sco_msg() do {show_msg(source, 0);}while(0)
#define handle_mic_msg() do {show_msg(source, 1);}while(0)
static void show_msg(Source source, int source_type);
static void print_msg(int msg_count, const uint16 *data, int msg_source);
#endif

static void msg_handler (Task appTask, MessageId id, Message msg);
static void initSetSpeechDataSource(Source src);
static void sendMessageMoreData(Task task, Source src, uint32 delay);
static void indicateFwdDataSource(Source src, source_type_t type);

static TaskData audioForwardTaskData = {.handler = msg_handler};
static Task audioForwardTask = &audioForwardTaskData;

static Source data_source_sco = NULL;
static Source data_source_mic = NULL;


bool disable_audio_forward(bool disable)
{
#if (FORWARD_AUDIO_TYPE & (FORWARD_AUDIO_MIC | FORWARD_AUDIO_SCO))
    uint16 set_data_format[] = { OPMSG_PASSTHROUGH_ID_DISABLE_AUDIO_FORWARD, disable };

    kymera_chain_handle_t sco_chain = appKymeraGetScoChain();

    if (sco_chain) {
        Operator passthrough = PanicZero(ChainGetOperatorByRole(sco_chain, OPR_CUSTOM_SCO_PASSTHROUGH));
        PanicZero(VmalOperatorMessage(passthrough, set_data_format,
                                      sizeof(set_data_format)/sizeof(set_data_format[0]), NULL, 0));
        return TRUE;
    }
#endif
    return FALSE;
}

void forwardAudioAndMic(kymera_chain_handle_t sco_chain)
{
    uint16 set_data_format[] = { OPMSG_PASSTHROUGH_ID_CHANGE_OUTPUT_DATA_TYPE, AUDIO_DATA_FORMAT_16_BIT };

#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_SCO )
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
#endif

#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_MIC )
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
#endif

#ifdef GAIA_TEST
    /* 5. notify gaia dialog have start*/
    gaiaStartNotify();
#endif
}

void disconnectAudioForward(kymera_chain_handle_t sco_chain) {
    /* Disconnect audio forward source */
#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_SCO )
    Source sco_audfwd_src = ChainGetOutput(sco_chain, EPR_AUDIO_SCOFWD_OUT);
    SourceUnmap(sco_audfwd_src);
#endif

#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_MIC )
    Source mic_audfwd_src = ChainGetOutput(sco_chain, EPR_AUDIO_MICFWD_OUT);
    SourceUnmap(mic_audfwd_src);
#endif
}


Task getAudioForwardTask(void)
{
    return audioForwardTask;
}

/*
 * Static Functions
*/
static void msg_handler (Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    switch (id) {
        case MESSAGE_MORE_DATA:
        {
            MessageMoreData * message = (MessageMoreData *)msg;
            Source source = message->source;

            if (source == data_source_sco) {
                handle_sco_msg();
            }
            else if (source == data_source_mic) {
                handle_mic_msg();
            }
            else {
                printf("ERROR: msg_handler: MESSAGE_MORE_DATA: INCORRECT SOURCE!\n");
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
        {
#ifdef GAIA_TEST
            handle_starot_gaia_msg(id, msg, &data_source_sco, &data_source_mic);
#endif
            break;
        }
    }
}

static void indicateFwdDataSource(Source src, source_type_t type)
{
    if (type > STYPE_MIC) return;

#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_SCO )
    if (STYPE_SCO == type) data_source_sco = src;
#endif

#if (FORWARD_AUDIO_TYPE & FORWARD_AUDIO_MIC )
    if (STYPE_MIC == type) data_source_mic = src;
#endif

    MAKE_AUDIO_MESSAGE( AUDIO_VA_INDICATE_DATA_SOURCE, message) ;
    message->data_src = src;
    MessageSend(audioForwardTask, AUDIO_VA_INDICATE_DATA_SOURCE, message);

#ifdef GAIA_TEST
    printf("Source src:%x, source_type_t type:%d\n", src, type);
	notifyGaiaDialogSource(data_source_sco, data_source_mic);
#endif

}

static void sendMessageMoreData(Task task, Source src, uint32 delay)
{
    MessageMoreData *message = PanicUnlessMalloc(sizeof(MessageMoreData));
    message->source = src;
    MessageCancelAll(task, MESSAGE_MORE_DATA);
    MessageSendLater(task, MESSAGE_MORE_DATA, message, delay);
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

#ifndef GAIA_TEST
static unsigned int msg_cnt_sco = 0;
static unsigned int msg_cnt_mic = 0;

static void show_msg(Source source, int source_type)
{
    int size = SourceSize(source);
    const uint16 *ptr = (const uint16*)SourceMap(source);

    if (0 == source_type) {
        msg_cnt_sco ++;
        print_msg(msg_cnt_sco, ptr, source_type);
    }
    else if (1 == source_type) {
        msg_cnt_mic ++;
        print_msg(msg_cnt_mic, ptr, source_type);
    }

    SourceDrop(source, size);
}

static void print_msg(int msg_count, const uint16 *data, int msg_source)
{
    if (0 == msg_count % 100) {
        if (0 == msg_source) printf("sco: ");
        else if (1 == msg_source) printf("mic: ");

        printf("cnt=%d [%04x %04x %04x %04x %04x %04x]\n",
               msg_count, data[0],data[1],data[2],data[3],data[4],data[5]);
    }
}
#endif
