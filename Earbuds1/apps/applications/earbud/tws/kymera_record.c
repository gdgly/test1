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

void appKymeraHandleInternalRecordStart(const KYMERA_INTERNAL_RECORD_T *msg)
{
    kymeraTaskData *theKymera = appGetKymera();

    DEBUG_LOGF("appKymeraHandleInternalRecord curstate=%d", appKymeraGetState());

    /* If there is a tone still playing at this point, it must be an interruptable tone, so cut it off */
    if (appKymeraRecordIsRun() == TRUE)
        return;

    switch (appKymeraGetState())
    {
        case KYMERA_STATE_A2DP_STREAMING:
        case KYMERA_STATE_IDLE:
            /* Need to set up audio output chain to play tone from scratch */
            appKymeraCreateRecordChain(msg->rate);

            forwardAudioAndMic(theKymera->chain_record_handle);

            ChainStart(theKymera->chain_record_handle);

            /* May need to exit low power mode to play tone simultaneously */
            appKymeraConfigureDspPowerMode(TRUE);
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
    switch (appKymeraGetState())
    {
        case KYMERA_STATE_A2DP_STREAMING:
        default:
        {
            Operator op = ChainGetOperatorByRole(theKymera->chain_record_handle, OPR_VOLUME_CONTROL);
            uint16 volume = volTo60thDbGain(0);
            OperatorsVolumeSetAuxGain(op, volume);

            ChainStop(theKymera->chain_record_handle);
            disconnectAudioForward(theKymera->chain_record_handle);

            appKymeraMicCleanup(appConfigScoMic1(), appConfigScoMic2());

            /* Disable external amplifier if required */
            ChainDestroy(theKymera->chain_record_handle);
            theKymera->chain_record_handle = NULL;
            theKymera->mic_params[0].gain = appConfigMic0Gain();
            theKymera->mic_params[1].gain = appConfigMic0Gain();
            appConfigScoMic1() = 0;
            appConfigScoMic2() = 1;
        }
        break;
    }

}
#endif
