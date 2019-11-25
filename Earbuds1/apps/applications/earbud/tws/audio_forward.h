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

enum {DATA_CLIENT_GAIA=0, DATA_CLIENT_COMMUPC};
typedef struct
{
    TaskData               data;
    Source      data_source_sco;
    Source      data_source_mic;
    uint16          data_client;
#ifndef GAIA_TEST
    unsigned int msg_cnt_sco;
    unsigned int msg_cnt_mic;
#endif
}AudioForwardTaskData;

void forwardSetDataClient(uint16 data_client);
void forwardAudioAndMic(kymera_chain_handle_t sco_chain);
void disconnectAudioForward(kymera_chain_handle_t sco_chain);
Task getAudioForwardTask(void);
void disable_audio_forward(bool disable);

#endif // AUDIO_FORWARD_H
