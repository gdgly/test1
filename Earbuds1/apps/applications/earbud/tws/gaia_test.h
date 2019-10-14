#ifndef GAIA_TEST_H
#define GAIA_TEST_H

#include <sink.h>
#include <source.h>
#include <stream.h>
#include <panic.h>
#include <audio.h>
#include <stdio.h>
#include <vmal.h>

#include "av_headset_gaia_starot.h"

void gaiaStartNotify(void);
//void handle_starot_gaia_msg(MessageId id, Message msg,
//                            Source* data_source_sco, Source* data_source_mic);
bool sendDataMessage(Source source, enum GAIA_AUDIO_TYPE type,
                     Source data_source_sco, Source data_source_mic);

void gaiaClearDropAudioSize(void);
int gaiaGetDropAudioSize(void);

#endif // GAIA_TEST_H
