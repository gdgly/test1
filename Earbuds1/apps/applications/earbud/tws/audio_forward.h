#ifndef AUDIO_FORWARD_H
#define AUDIO_FORWARD_H

#include <source.h>
#include <chain.h>

typedef enum soruce_type{
	STYPE_SCO,
	STYPE_MIC
}source_type_t;


enum audio_forward_messages
{
    AUDIO_FWD_DISABLE = AUDIO_FWD_MESSAGE_BASE
};

typedef struct
{
    bool disable;
} AUDIO_FWD_DISABLE_T;


typedef struct
{
    TaskData               data;
    Source      data_source_sco;
    Source      data_source_mic;
#ifndef GAIA_TEST
    unsigned int msg_cnt_sco;
    unsigned int msg_cnt_mic;
#endif
}AudioForwardTaskData;


void forwardAudioAndMic(kymera_chain_handle_t sco_chain);
void disconnectAudioForward(kymera_chain_handle_t sco_chain);
Task getAudioForwardTask(void);
void disable_audio_forward(bool disable);

#endif // AUDIO_FORWARD_H
