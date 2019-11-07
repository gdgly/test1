
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
#define GAIA_COMMAND_STAROT_SET_DOUBLE_CLICK_SETTING 0X5104
#define GAIA_COMMAND_STAROT_NOTIFY_CASE_STATUS 0X5105

///////////////////////////控制类命令///////////////////////////////
enum {
    GAIA_COMMAND_STAROT_CONTROL_CALL_DIALOG = 0X5400,                         // 拨打电话
    GAIA_COMMAND_STAROT_CONTROL_ACCEPT_DIALOG = 0X5401,                       // 接听电话
    GAIA_COMMAND_STAROT_CONTROL_REJECT_DIALOG = 0X5402,                       // 拒绝电话
    GAIA_COMMAND_STAROT_CONTROL_PREVIOUS_MUSIC= 0X5403,                       // 上一首音乐
    GAIA_COMMAND_STAROT_CONTROL_NEXT_MUSIC= 0X5404,                           // 下一首音乐
    GAIA_COMMAND_STAROT_CONTROL_VOLUME_SET= 0X5405,                           // 音量设置
};
///////////////////////////base info///////////////////////////////
enum {
    GAIA_COMMAND_STAROT_BASE_INFO_GET_VERSION = 0X5500,                       // 获取版本
    GAIA_COMMAND_STAROT_BASE_INFO_GET_DOUBLE_CLIENT_CONFIG= 0X5501,           // 获取双击设置
    GAIA_COMMAND_STAROT_BASE_INFO_SET_DOUBLE_CLIENT_CONFIG= 0X5502,           // 设置双击设置
    GAIA_COMMAND_STAROT_BASE_INFO_NOTIFY_CASE_STATUS = 0X5503,                // 通知充电盒状态
    GAIA_COMMAND_STAROT_BASE_INFO_NOTIFY_POWER_POSITION_CONNECTION = 0X5504,  // 通知电量、位置、连接信息
    GAIA_COMMAND_STAROT_BASE_INFO_APPGET_POWER_POSITION_CONNECTION = 0X5505,  // app获取电量、位置、连接信息
};
/////////////////////////////助手控制////////////////////////////////
enum {
    GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START = 0X5200,                     // 请求开始助手
    GAIA_COMMAND_STAROT_AI_CONTROL = 0X5201,                                  // 助手控制录音
    GAIA_COMMAND_STAROT_AI_AUDIO_TO_APP = 0X5202,                             // 助手音频Device->App
    GAIA_COMMAND_STAROT_AI_AUDIO_TO_DEVICE = 0X5203,                          // 助手音频App->Device
    GAIA_COMMAND_STAROT_AI_AUDIO_TO_DEVICE_ACK  = 0X5204,                     // 音频确认包
    GAIA_COMMAND_STAROT_AI_BEGIN_RECORD = 0X5205,                             // 开始录音
    GAIA_COMMAND_STAROT_AI_END_RECORD = 0X5206,                               // 停止录音
    GAIA_CONNECT_STAROT_RECORD_STOP_REPORT = 0X5207,                          // 强制停止录音
};

/////////////////////////////测试与生产///////////////////////////////
enum {
    GAIA_COMMAND_STAROT_TEST_PRODUCT_REST = 0X5600,                           // 恢复出厂设置
};

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

enum GAIA_AUDIO_TRANSFORM_FLAG {
    TRANSFORM_NONE = 0,
    TRANSFORM_COMING = 1,
    TRANSFORM_CANT = 2,
    DIALOG_CAN_TRANSFORM = GAIA_COMMAND_STAROT_CALL_AUDIO_IND,
    RECORD_CAN_TRANSFORM = GAIA_COMMAND_STAROT_AI_AUDIO_TO_APP,
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

#define CALL_AUDIO_IND(cmd) ((cmd) == GAIA_COMMAND_STAROT_CALL_AUDIO_IND || (cmd) == GAIA_COMMAND_STAROT_AI_AUDIO_TO_APP)

void starotGaiaInit(void);

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message);

void starotGaiaParseMessageMoreSpace(void);

bool starotGaiaSendAudio(GAIA_STAROT_AUDIO_IND_T *message);

void starotNotifyAudioForward(bool st, uint8 flag);

void notifyGaiaDialogSource(Source speaker, Source mic);

uint8 starotGaiaTransGetAudioType(void);

void starotGaiaDefaultParse( MessageId id, Message message);

void starotGaiaParseCfm(const GAIA_SEND_PACKET_CFM_T *m);

void starotGaiaSetTransportType(gaia_transport_type gaiaTransportType);
//-----------------------------------------------------
// 定义GAIA与UI、DSP关于电话时的消息
//-----------------------------------------------------

enum {
    STAROT_DIALOG_STATUS = AV_GAIA_MESSAGE_BASE + 0X20,   // ui -> gaia 通话状态
    STAROT_DIALOG_TYPE,                                   // ui -> gaia 报告当前通话是电话还是WX其它通话
    STAROT_DIALOG_USER_ACCEPT_RECORD,                     // gaia -> (ui & dsp) 用户请求录音
    STAROT_DIALOG_AUDIO_DATA,                             // dsp -> gaia 请求传输数据
    STAROT_DIALOG_USER_REJECT_RECORD,                     // gaia -> (ui & dsp) 用户请求录音
    STAROT_DIALOG_DEVICE_REQUEST_RECORD,                  // ui -> gaia 设备请求用户开始录音
    STAROT_DIALOG_DEVICE_REQUEST_REJECT_RECORD,           // ui -> gaia 设备请求用户停止录音
    STAROT_DIALOG_AUDIO_DEVICE_PAUSE,                     // ui -> gaia 音频设备发生切换 暂停
    STAROT_DIALOG_AUDIO_DEVICE_CONTINUE,                  // ui -> gaia 音频设备发生切换 继续
    STAROT_DIALOG_CALL_ATTR_TIMEOUT,                      // gaia -> gaia 属性发送超时，如果还在通话中，继续发送
    STAROT_DIALOG_CALL_BEGIN_TIMEOUT,                     // gaia -> gaia 电话开始发送命令超时
    STAROT_DIALOG_CALL_END_TIMEOUT,                       // gaia -> gaia 电话结束发送超时
    STAROT_DIALOG_CASE_STAT,                              // ui -> gaia 盒子当前信息
    STAROT_DIALOG_CASE_VER,                               // ui -> gaia 盒子当前版本
    STAROT_AI_USER_START_RECORD,                          // ui -> (ui & dsp) AI请求录音
    STAROT_AI_USER_STOP_RECORD,                           // ui -> (ui & dsp) AI停止录音
    STAROT_RECORD_STOP_STATUS_REPORT,                     // dsp -> ui 上报停止录音状态
    STAROT_RECORD_RETURN_THREE_POWER,                     // gaia -> ui App主动获取电量状态
    STAROT_NOTIFY_STATUS,                                 // ui -> gaia 状态发生变化
};

#define STAROT_COMMAND_TIMEOUT 1000 // 命令超时时间

/////////////////////////////////////////命令，超时，重发//////////////////////////////////////////
struct StarotResendCommand_T {
    uint16 command;
    uint16 len;   /// payload的长度
    uint8 payload[4];
};



typedef struct StarotResendCommand_T StarotResendCommand;

StarotResendCommand* starotResendCommandInit(uint16 command, uint16 len, uint8* payload);
StarotResendCommand* starotResendCommandDo(StarotResendCommand* resendCommand, bool stillNeedResend);
/////////////////////////////////////////////////////////////////////////////////////////////////

#endif // AV_HEADSET_GAIA_STAROT_H
