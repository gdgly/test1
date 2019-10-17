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
#include "gaia_test.h"

unsigned nowSendStatus = 0;
int dissNum = 0;
extern uint16 bufferSendUnit;

/// true：发送数据到GAIA的TASK
/// false：没有发送消息到GAIA的TASK
bool sendDataMessage(Source source, enum GAIA_AUDIO_TYPE type,
                     Source data_source_sco, Source data_source_mic) {
    UNUSED(data_source_mic);
    UNUSED(data_source_sco);
    int size = SourceSize(source);

    int dropUnit = 10;
    /// 丢弃过多的数据，防止数据过多，导致source不可以使用
    if (size > (dropUnit * bufferSendUnit)) {
        int drop = size - (dropUnit * bufferSendUnit);
        drop = drop / bufferSendUnit * bufferSendUnit;
        size -= drop;
        dissNum += drop;
        DEBUG_LOG("drop size %d", drop);
        SourceDrop(source, drop);
    }
    dissNum += 1;

    if (NULL == appGetGaia()->transport) {
        DEBUG_LOG("sendDataMessage,type=%d Drop:%d no connect", type, size);
        SourceDrop(source, size);
    } else if (size >= bufferSendUnit) {
        GAIA_STAROT_AUDIO_IND_T* starot = PanicUnlessMalloc(sizeof(GAIA_STAROT_AUDIO_IND_T));
        starot->command = STAROT_DIALOG_AUDIO_DATA;
        starot->source = source;
        starot->audioType = type;
        starot->data = NULL;
        starot->len = size;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, starot);
        nowSendStatus = 1;
        return TRUE;
    }
    return FALSE;
}

void gaiaClearDropAudioSize(void) {
    dissNum = 0;
}

int gaiaGetDropAudioSize(void) {
    return dissNum;
}
