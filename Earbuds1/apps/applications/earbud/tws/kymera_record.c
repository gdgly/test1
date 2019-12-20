/*!
\copyright  Copyright (c) 2017-2018  Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file
\brief      Kymera record
*/

#include "av_headset_kymera_private.h"

#include "chains/chain_assistant.h"
#include "tws/audio_forward.h"
#include "cap_id_prim.h"

#ifdef CONFIG_REC_ASSISTANT

bool appKymeraRecordIsRun(void)
{
    kymeraTaskData *theKymera = appGetKymera();
    return (theKymera->chain_record_handle != NULL) ? TRUE : FALSE;
}


static void appKymeraCreateRecordChain(uint32 rate)
{
    kymeraTaskData *theKymera = appGetKymera();
    Source mic, mic2;
    kymera_chain_handle_t chain;

    /* Create chain */
    chain = ChainCreate(&chain_assistant_config);
    theKymera->chain_record_handle = chain;
 //   appKymeraConfigureOutputChainOperators(chain, rate, kick_period, buffer_size, volume);

#if 1
    /* Get microphone sources */
    appKymeraMicSetup(appConfigScoMic1(), &mic, appConfigScoMic2(), &mic2, rate);
#else
    /* Configure the DAC channel */
    mic = StreamAudioSource(appConfigLeftAudioHardware(), appConfigLeftAudioInstance(), appConfigLeftAudioChannel());
    PanicFalse(SourceConfigure(mic, STREAM_CODEC_OUTPUT_RATE, rate));
    PanicFalse(SourceConfigure(mic, STREAM_RM_ENABLE_DEFERRED_KICK, 0));
#endif

    /* Connect chain output to the DAC */
    ChainConnect(chain);
    PanicFalse(ChainConnectInput(chain, mic, EPR_VOLUME_AUX));

    return;
}

extern bool VmalOperatorMessage(uint16 opid, const void * send_msg, uint16 send_len,
                                void * recv_msg, uint16 recv_len);
#define GAIN_DB_TO_Q6N_SF (11146541)
#define GAIN_DB(x)      ((int32)(GAIN_DB_TO_Q6N_SF * (x)))

void appKymeraHandleInternalRecordStart(const KYMERA_INTERNAL_RECORD_T *msg)
{
    const int32 initial_gain = GAIN_DB(18);
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalRecord curstate=%d", appKymeraGetState());

    /* If there is a tone still playing at this point, it must be an interruptable tone, so cut it off */
    if (appKymeraRecordIsRun() == TRUE)
        return;

    switch (appKymeraGetState())
    {
        case KYMERA_STATE_TONE_PLAYING:
        case KYMERA_STATE_A2DP_STREAMING:
        case KYMERA_STATE_IDLE:
            /* Need to set up audio output chain to play tone from scratch */
            appKymeraCreateRecordChain(msg->rate);

            Operator passthrough = ChainGetOperatorByRole(theKymera->chain_record_handle, OPR_SWITCHED_PASSTHROUGH_CONSUMER);
            uint16 set_gain[] = { OPMSG_COMMON_ID_SET_PARAMS, 1, 1, 1, UINT32_MSW(initial_gain), UINT32_LSW(initial_gain) };
            PanicFalse(VmalOperatorMessage(passthrough, set_gain, sizeof(set_gain)/sizeof(set_gain[0]), NULL, 0));

            forwardAudioAndMic(theKymera->chain_record_handle);

            ChainStart(theKymera->chain_record_handle);

            /* May need to exit low power mode to play tone simultaneously */
            appKymeraConfigureDspPowerMode(TRUE);

            // 允许DSP数据传到APP
            disable_audio_forward(FALSE);
            break;

        default:
            /* Unknown state / not supported */
            DEBUG_LOGF("appKymeraHandleInternalRecord, unsupported state %u", appKymeraGetState());
//            Panic(); //音乐必须停止才启动录音，先注释掉
            break;
    }

}

void appKymeraRecordStop(void)
{
    kymeraTaskData *theKymera = appGetKymera();

    /* Exit if there isn't a tone or prompt playing */
    if (appKymeraRecordIsRun() == FALSE)
        return;

    DEBUG_LOGF("appKymeraRecordStop, state %u", appKymeraGetState());
    disable_audio_forward(TRUE);
    switch (appKymeraGetState())
    {
        case KYMERA_STATE_A2DP_STREAMING:
        default:
        {
#if 0
            Operator op = ChainGetOperatorByRole(theKymera->chain_record_handle, OPR_VOLUME_CONTROL);
            uint16 volume = volTo60thDbGain(0);
            OperatorsVolumeSetAuxGain(op, volume);
#endif

            ChainStop(theKymera->chain_record_handle);
            disconnectAudioForward(theKymera->chain_record_handle);

            appKymeraMicCleanup(appConfigScoMic1(), appConfigScoMic2());

            /* Disable external amplifier if required */
            ChainDestroy(theKymera->chain_record_handle);
            theKymera->chain_record_handle = NULL;
        }
        break;
    }

}
#endif
