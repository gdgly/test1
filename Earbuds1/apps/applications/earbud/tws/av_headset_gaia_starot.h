
#ifndef AV_HEADSET_GAIA_STAROT_H
#define AV_HEADSET_GAIA_STAROT_H

#include <gaia.h>
#include <../av_headset.h>

#ifdef GAIA_TEST

////////////////////////////EVENT//////////////////////////////
enum {
    /// 临时，不能这样定义，会冲突
            GAIA_STAROT_AUDIO_INTERVAL = GAIA_MESSAGE_TOP + 1,
    GAIA_STAROT_RESEND_AUDIO,
};
///////////////////////////COMMAND/////////////////////////////
#define GAIA_COMMAND_STAROT_FIRST_COMMAND (0x5001)
#define GAIA_COMMAND_STAROT_START_SEND_TIMER (0x5002)
#define GAIA_COMMAND_STAROT_STOP_SEND_TIMER (0x5003)
#define GAIA_COMMAND_STAROT_TRANS_AUDIO (0x5004)
#define GAIA_COMMAND_STAROT_CALL_BEGIN (0x5005)
#define GAIA_COMMAND_STAROT_CALL_END (0x5006)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_IND (0x5007)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_CFM (0x5008)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_END (0x5009)
#define GAIA_COMMAND_STAROT_CALL_SOURCE_IND (0x500a)

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message);

void starotGaiaParseMessageMoreSpace(void);

#define W16(x) (((*(x)) << 8) | (*((x) + 1)))
#define GAIA_OFFS_VENDOR_ID (4)
#define GAIA_OFFS_COMMAND_ID (6)
#define GAIA_OFFS_PAYLOAD_LENGTH (3)
#define GAIA_OFFS_VERSION (1)
#define GAIA_OFFS_PAYLOAD (8)

enum GAIA_TRANSFORM_AUDIO_STATUS {
    GAIA_TRANSFORM_AUDIO_IDLE,
    GAIA_TRANSFORM_AUDIO_ING,
    GAIA_TRANSFORM_AUDIO_WAIT_MORE_SPACE,
};

enum GAIA_AUDIO_TYPE {
    GAIA_AUDIO_SPEAKER = 1,
    GAIA_AUDIO_MIC = 2
};

typedef struct {
    uint16 command;
    Source source;
    uint8 *data;
    uint16 len;
    uint16 audioType;
} GAIA_STAROT_AUDIO_T;

typedef GAIA_STAROT_AUDIO_T GAIA_STAROT_AUDIO_IND_T;
typedef GAIA_STAROT_AUDIO_T GAIA_STAROT_AUDIO_CFM_T;

typedef struct {
    uint16 command;
    Source speark;
    Source mic;
} GAIA_STAROT_DIALOG_SOURCE;

typedef GAIA_STAROT_DIALOG_SOURCE GAIA_STAROT_DIALOG_SOURCE_T;

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T *message);

void starotNotifyAudioForward(bool st, uint8 flag);

void notifyGaiaDialogSource(Source speaker, Source mic);

#endif
#endif // AV_HEADSET_GAIA_STAROT_H
