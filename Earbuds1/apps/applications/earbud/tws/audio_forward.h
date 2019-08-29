#ifndef AUDIO_FORWARD_H
#define AUDIO_FORWARD_H

#include <source.h>
#include <chain.h>

typedef enum soruce_type{
	STYPE_SCO,
	STYPE_MIC
}source_type_t;

void forwardAudioAndMic(kymera_chain_handle_t sco_chain);
void disconnectAudioForward(kymera_chain_handle_t sco_chain);
Task getAudioForwardTask(void);
bool disable_audio_forward(bool disable);

#endif // AUDIO_FORWARD_H
