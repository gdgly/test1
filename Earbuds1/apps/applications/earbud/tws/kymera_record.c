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

#define chain_record_handle   chainu.sco_handle     // 使用soc来保存当前的CHAIN


static void appKymeraCreateRecordChain(uint32 rate)
{
    kymeraTaskData *theKymera = appGetKymera();
    Source mic;
    kymera_chain_handle_t chain;

    /* Create chain */
    chain = ChainCreate(&chain_assistant_config);
    theKymera->chain_record_handle = chain;
 //   appKymeraConfigureOutputChainOperators(chain, rate, kick_period, buffer_size, volume);

#if 1
    /* Get microphone sources */
    appKymeraMicSetup(appConfigScoMic1(), &mic, appConfigScoMic2(), NULL, rate);
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

    DEBUG_LOGF("appKymeraHandleInternalRecord");

    /* If there is a tone still playing at this point, it must be an interruptable tone, so cut it off */
    appKymeraRecordStop();

    switch (appKymeraGetState())
    {
        case KYMERA_STATE_IDLE:
            /* Need to set up audio output chain to play tone from scratch */
            appKymeraCreateRecordChain(msg->rate);

            forwardAudioAndMic(theKymera->chain_record_handle);

            appKymeraExternalAmpControl(TRUE);
            ChainStart(theKymera->chain_record_handle);

            appKymeraSetState(KYMERA_STATE_AUDIO_RECORD);
            /* May need to exit low power mode to play tone simultaneously */
            appKymeraConfigureDspPowerMode(TRUE);
            break;

        default:
            /* Unknown state / not supported */
            DEBUG_LOGF("appKymeraHandleInternalRecord, unsupported state %u", appKymeraGetState());
            Panic();
            break;
    }

}

void appKymeraRecordStop(void)
{
    kymeraTaskData *theKymera = appGetKymera();

    /* Exit if there isn't a tone or prompt playing */
    if (!theKymera->chain_record_handle)
        return;

    DEBUG_LOGF("appKymeraRecordStop, state %u", appKymeraGetState());
    switch (appKymeraGetState())
    {
        case KYMERA_STATE_AUDIO_RECORD:
        {
            Operator op = ChainGetOperatorByRole(theKymera->chain_record_handle, OPR_VOLUME_CONTROL);
            uint16 volume = volTo60thDbGain(0);
            OperatorsVolumeSetAuxGain(op, volume);

            ChainStop(theKymera->chain_record_handle);
            disconnectAudioForward(theKymera->chain_record_handle);

            appKymeraMicCleanup(appConfigScoMic1(), appConfigScoMic2());

            /* Disable external amplifier if required */
            appKymeraExternalAmpControl(FALSE);
            ChainDestroy(theKymera->chain_record_handle);
            theKymera->chain_record_handle = NULL;

            /* Move back to idle state */
            appKymeraSetState(KYMERA_STATE_IDLE);
        }
        break;

        case KYMERA_STATE_IDLE:
            break;
        default:
            /* Unknown state / not supported */
            DEBUG_LOGF("appKymeraRecordStop, unsupported state %u", appKymeraGetState());
            break;
    }

}
#endif
