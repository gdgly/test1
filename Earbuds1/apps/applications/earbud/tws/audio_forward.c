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

#ifdef GAIA_TEST
extern uint16 bufferSendUnit;

#define AUDIO_CORE_0             (0)
#define AUDIO_DATA_FORMAT_16_BIT (0)

static void msg_handler (Task appTask, MessageId id, Message msg);
static void initSetSpeechDataSource(Source src);
static void sendMessageMoreData(Task task, Source src, uint32 delay);
static void indicateFwdDataSource(Source src, source_type_t type);
/// true：发送数据到GAIA的TASK
/// false：没有发送消息到GAIA的TASK
static bool sendDataMessage(Source source, enum GAIA_AUDIO_TYPE type);

static TaskData audioForwardTaskData = {.handler = msg_handler};
static Task audioForwardTask = &audioForwardTaskData;

static Source data_source_sco = NULL;
static Source data_source_mic = NULL;


#define  OPMSG_PASSTHROUGH_ID_DISABLE_AUDIO_FORWARD (0x000C)

bool disable_audio_forward(bool disable) {
    uint16 set_data_format[] = { OPMSG_PASSTHROUGH_ID_DISABLE_AUDIO_FORWARD, disable };

    kymera_chain_handle_t sco_chain = appKymeraGetScoChain();

    if (sco_chain) {
        Operator passthrough = PanicZero(ChainGetOperatorByRole(sco_chain, OPR_CUSTOM_SCO_PASSTHROUGH));
        PanicZero(VmalOperatorMessage(passthrough, set_data_format,
                                      sizeof(set_data_format)/sizeof(set_data_format[0]), NULL, 0));
        return TRUE;
    }

    return FALSE;
}

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

    /* 5. notify gaia dialog have start*/
    if (NULL != appGetGaia()->transport) {
        GAIA_STAROT_IND_T* starot = PanicUnlessNew(GAIA_STAROT_IND_T);
        starot->command = GAIA_COMMAND_STAROT_CALL_BEGIN;
        starot->payloadLen = 0;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
    }
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

static bool sendDataMessage(Source source, enum GAIA_AUDIO_TYPE type) {
    int size = SourceSize(source);
//    const uint16 *ptr = (const uint16*)SourceMap(source);

#ifdef GAIA_TEST
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
#else
    (void)bufferSendUnit;
    DEBUG_LOG("sendDataMessage,type=%d Drop:%d", type, size);
    SourceDrop(source, size);
#endif
    return FALSE;
}

static void msg_handler (Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    switch (id) {
    case MESSAGE_MORE_DATA:
    {
        MessageMoreData * message = (MessageMoreData *)msg;
        Source source = message->source;
#ifndef GAIA_TEST
        int size = SourceSize(source);
        const uint16 *ptr = (const uint16*)SourceMap(source);
#endif
        if (source == data_source_sco)
        {
#ifndef GAIA_TEST
            static unsigned more_loops_sco = 0, data_cnt_sco = 0;

            data_cnt_sco += size;

            if(0 == (++more_loops_sco) % 100)
                printf("sco: get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
                       more_loops_sco, size, data_cnt_sco, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);
#endif
//            int size = SourceSize(source);
//            SourceDrop(source, size);
            sendDataMessage(message->source, GAIA_AUDIO_SPEAKER);
        }
        else if (source == data_source_mic)
        {
#ifndef GAIA_TEST
            static unsigned more_loops_mic = 0, data_cnt_mic = 0;

            data_cnt_mic += size;

            if(0 == (++more_loops_mic) % 100)
                printf("mic: get=%d data=%d cnt=%d [%04x %04x %04x %04x %04x %04x\n",
                       more_loops_mic, size, data_cnt_mic, ptr[0],ptr[1],ptr[2],ptr[3],ptr[4],ptr[5]);
#endif
            sendDataMessage(message->source, GAIA_AUDIO_MIC);
        }
        else
            printf("ERROR: msg_handler: MESSAGE_MORE_DATA: INCORRECT SOURCE!\n");
#ifndef GAIA_TEST
        SourceDrop(source, size);
#endif
        break;
    }

#ifdef GAIA_TEST
    case GAIA_STAROT_COMMAND_IND:
    {
        /// 确认发送的消息
        GAIA_STAROT_AUDIO_CFM_T* message = (GAIA_STAROT_AUDIO_CFM_T*)msg;
        nowSendStatus = 0;
         if (NULL != message && message->command == GAIA_COMMAND_STAROT_CALL_AUDIO_END) {
            printf("diss audio bytes is : %d\n", dissNum);
            dissNum = 0;
            data_source_sco = NULL;
            data_source_mic = NULL;
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

    printf("Source src:%x, source_type_t type:%d\n", src, type);
    if (NULL != data_source_sco && NULL != data_source_mic) {
        notifyGaiaDialogSource(data_source_sco, data_source_mic);
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

#endif
