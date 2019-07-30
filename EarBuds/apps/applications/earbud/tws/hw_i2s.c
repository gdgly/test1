#include <stdio.h>
#include <sink.h>
#include <source.h>
#include <stream.h>
#include <operator.h>
#include <hydra_macros.h>

#include <panic.h>
#include <pio.h>
#include <ps.h>
#include <string.h>
#include <boot.h>
#include <os.h>
#include <message.h>
#include <app/message/system_message.h>
#include <vmal.h>
#include <audio_mclk.h>
#include <opmsg_prim.h>
#include <pmalloc.h>

#include "public.h"

void SourceI2SConfigure(Source source)
{
    AudioMasterClockConfigure(FALSE, 0);
    PanicFalse(SourceConfigure(source, STREAM_PCM_MASTER_MODE, 1));

    PanicFalse(SourceConfigure(source, STREAM_PCM_MASTER_CLK_SOURCE , CLK_SOURCE_TYPE_MCLK));
    PanicFalse(SourceConfigure(source, STREAM_PCM_MASTER_MCLK_MULT, 768));

    PanicFalse(SourceConfigure(source, STREAM_PCM_SHORT_SYNC_ENABLE, 1));

    /* set the bit clock (BCLK) rate for master mode */
    PanicFalse(SourceConfigure(source, STREAM_PCM_MASTER_CLOCK_RATE, 2048000));

    /* set the word clock (SYNC) rate of the dsp audio data */
    PanicFalse(SourceConfigure(source, STREAM_PCM_SYNC_RATE, 16000));

    /* PCM slot count. Valid range 0 to 4 inclusive. If 0, slot count will be
     * derived from master clock  and synchronisation rate.*/
    PanicFalse(SourceConfigure(source, STREAM_PCM_SLOT_COUNT, 0));

    /* Enable PCM LSB first - Transmit data LSB first.*/
    PanicFalse(SourceConfigure(source, STREAM_PCM_LSB_FIRST_ENABLE, 0));

    /*!< Enable PCM long length sync - Set PCM_SYNC to 8 or 16 PCM_CLK cycles.*/
    PanicFalse(SourceConfigure(source, STREAM_PCM_LONG_LENGTH_SYNC_ENABLE, 1));

    /*!< PCM sample format - Valid values: 0 (13 bits in 16 cycle slot duration),
    1 (16 bits in 16 cycle slot duration), 2 (8 bits in 16 cycle slot duration),
    3 (8 bits in 8 cycle slot duration).*/
    PanicFalse(SourceConfigure(source, STREAM_PCM_SAMPLE_FORMAT, 1));

    /*!< Enable PCM sample rising edge - Sample PCM_DATA on rising edge of PCM_CLK.*/
    PanicFalse(SourceConfigure(source, STREAM_PCM_SAMPLE_RISING_EDGE_ENABLE, 1));

    PanicFalse(SourceConfigure(source, STREAM_AUDIO_SAMPLE_SIZE, 16));
}


void SinkI2SConfigure(Sink sink)
{
    AudioMasterClockConfigure(FALSE, 0);
    PanicFalse(SinkConfigure(sink, STREAM_PCM_MASTER_MODE, 1));

    PanicFalse(SinkConfigure(sink, STREAM_PCM_MASTER_CLK_SOURCE , CLK_SOURCE_TYPE_MCLK));
    PanicFalse(SinkConfigure(sink, STREAM_PCM_MASTER_MCLK_MULT, 768));

    PanicFalse(SinkConfigure(sink, STREAM_PCM_SHORT_SYNC_ENABLE, 1));

    /* set the bit clock (BCLK) rate for master mode */
    PanicFalse(SinkConfigure(sink, STREAM_PCM_MASTER_CLOCK_RATE, 2048000));

    /* set the word clock (SYNC) rate of the dsp audio data */
    PanicFalse(SinkConfigure(sink, STREAM_PCM_SYNC_RATE, 16000));

    /* PCM slot count. Valid range 0 to 4 inclusive. If 0, slot count will be
     * derived from master clock  and synchronisation rate.*/
    PanicFalse(SinkConfigure(sink, STREAM_PCM_SLOT_COUNT, 0));

    /* Enable PCM LSB first - Transmit data LSB first.*/
    PanicFalse(SinkConfigure(sink, STREAM_PCM_LSB_FIRST_ENABLE, 0));

    /*!< Enable PCM long length sync - Set PCM_SYNC to 8 or 16 PCM_CLK cycles.*/
    PanicFalse(SinkConfigure(sink, STREAM_PCM_LONG_LENGTH_SYNC_ENABLE, 1));

    /*!< PCM sample format - Valid values: 0 (13 bits in 16 cycle slot duration),
    1 (16 bits in 16 cycle slot duration), 2 (8 bits in 16 cycle slot duration),
    3 (8 bits in 8 cycle slot duration).*/
    PanicFalse(SinkConfigure(sink, STREAM_PCM_SAMPLE_FORMAT, 1));

    /*!< Enable PCM sample rising edge - Sample PCM_DATA on rising edge of PCM_CLK.*/
    PanicFalse(SinkConfigure(sink, STREAM_PCM_SAMPLE_RISING_EDGE_ENABLE, 1));

    PanicFalse(SinkConfigure(sink, STREAM_AUDIO_SAMPLE_SIZE, 16));
}

void SourceSinkI2SMasterClockEnable(void *source_sink, int isSource, int enable)
{
// 调用就失败
// PanicFalse(SourceConfigure(source, STREAM_ANC_INPUT, AUDIO_ANC_PATH_ID_NONE));

    if(isSource) {
        Source source = (Source)source_sink;
        if(enable)
            PanicFalse(SourceMasterClockEnable(source, MCLK_ENABLE_OUTPUT));
        else
            PanicFalse(SourceMasterClockEnable(source, MCLK_DISABLE));
    }
    else {
        Sink sink = (Sink)source_sink;
        if(enable)
            PanicFalse(SinkMasterClockEnable(sink, MCLK_ENABLE_OUTPUT));
        else
            PanicFalse(SinkMasterClockEnable(sink, MCLK_DISABLE));
    }
}
