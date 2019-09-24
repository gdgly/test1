
#ifndef AV_HEADSET_GAIA_STAROT_H
#define AV_HEADSET_GAIA_STAROT_H

#include <gaia.h>
#include <../av_headset.h>

enum GAIA_AUDIO_TYPE {
    GAIA_AUDIO_SPEAKER = 1,
    GAIA_AUDIO_MIC = 2
};


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

///////////////////////////dialog//////////////////////////////////
/// 电话开始
#define GAIA_COMMAND_STAROT_CALL_BEGIN (0x5005)
/// 电话结束
#define GAIA_COMMAND_STAROT_CALL_END (0x5006)

#define GAIA_COMMAND_STAROT_CALL_AUDIO_IND (0x5007)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_CFM (0x5008)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_END (0x5009)
#define GAIA_COMMAND_STAROT_CALL_SOURCE_IND (0x500a)

#define GAIA_COMMAND_STAROT_START_TRANS_AUDIO_IND (0x500b)
#define GAIA_COMMAND_STAROT_END_TRANS_AUDIO_IND (0x500c)
#define GAIA_COMMAND_STAROT_CALL_AUDIO_DEVICE (0X500D)
#define GAIA_COMMAND_STAROT_CALL_DEVICE_TRANS_AUDIO_REQUEST (0X500E)
#define GAIA_COMMAND_STAROT_CALL_ATTR (0X500F)

///////////////////////////bond code///////////////////////////////
#define GAIA_COMMAND_STAROT_SET_BOND_CODE_IND (0x5100)
#define GAIA_COMMAND_STAROT_CHECK_BOND_CODE_IND (0x5101)



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

enum GAIA_DIALOG_STATUS {
    DIALOG_NONE = 0,
    DIALOG_COMING = 1,
    DIALOG_CAN_TRANSFORM = 2
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


//-----------------------------------------------------
// 定义GAIA与UI、DSP关于电话时的消息
//-----------------------------------------------------
#define STAROT_DIALOG_STATUS 0x1001                  /// ui -> gaia 通话状态
#define STAROT_DIALOG_USER_ACCEPT_RECORD 0x1002    /// gaia -> (ui & dsp) 用户请求录音
#define STAROT_DIALOG_AUDIO_DATA 0x1003             /// dsp -> gaia 请求传输数据
#define STAROT_DIALOG_USER_REJECT_RECORD 0x1004    /// gaia -> (ui & dsp) 用户请求录音
#define STAROT_DIALOG_DEVICE_REQUEST__RECORD 0x1005    /// ui -> gaia 设备请求用户开始录音
#define STAROT_DIALOG_DEVICE_REQUEST_REJECT_RECORD 0x1007    /// ui -> gaia 设备请求用户停止录音
#define STAROT_DIALOG_AUDIO_DEVICE_PAUSE 0x1009    /// ui -> gaia 音频设备发生切换 暂停
#define STAROT_DIALOG_AUDIO_DEVICE_CONTINUE 0x100b    /// ui -> gaia 音频设备发生切换 继续

#endif // AV_HEADSET_GAIA_STAROT_H
