
#ifndef AV_HEADSET_GAIA_STAROT_H
#define AV_HEADSET_GAIA_STAROT_H

#include <gaia.h>
#include <../av_headset.h>

// region task数据

typedef struct {
    uint8 callerNumber[32];
    uint16 callerLen;
    uint16 connectLock; // 0:无锁 1:加锁
} subGaiaTaskData;

void subGaiaTaskInit(void);
subGaiaTaskData* subGaiaGetTaskData(void);
// endregion

// region 联系人信息

const uint8* subGaiaGetCaller(uint16* len);
void subGaiaClearCaller(void);
void subGaiaSetCaller(uint8* data, uint16 len);

// endregion

// region 使用conditionally发送消息

///成功发送校验码之后，设置为unlock状态。
enum {
    SUB_GAIA_CONNECT_UNLOCK = 0,
    SUB_GAIA_CONNECT_LOCK = 1
};

void subGaiaSetConnectUnlock(void);
void subGaiaClearConnectUnlock(void);
uint16* subGaiaGetConnectLock(void);
bool subGaiaIsConnectLock(void);

// endregion


#define CALL_IN_ACTIVE          (1 << 0)
#define CALL_OUT_ACTIVE         (1 << 1)
#define CALL_ACTIVE             (1 << 2)
#define CALL_INACTIVE           (1 << 3)
#define CALL_IN_INACTIVE        (1 << 4)
#define CALL_OUT_INACTIVE       (1 << 5)

enum GAIA_AUDIO_TYPE {
    GAIA_AUDIO_SPEAKER = 1,
    GAIA_AUDIO_MIC = 2
};

/// 消息的来源
enum MessageFromSource {
    MESSAGE_FROM_APP,    /// 从APP发送来的消息
    MESSAGE_FROM_PEER    /// 从Peer耳机转发的消息
};

typedef struct
{
    uint16 command;
    uint16 payloadLen;
    uint16 messageFrom;
    uint8 payload[4];
} GAIA_STAROT_CONFIG_IND_T;
typedef GAIA_STAROT_CONFIG_IND_T GAIA_STAROT_CONFIG_IND;

enum {
    ASSISTANT_TYPE_APP = 1,
    ASSISTANT_TYPE_SYSTEM = 2
};

typedef struct
{
    uint16 command;
    uint8 messageFrom;
    uint8 apollo_enable;
    uint32 timestamp;
    uint8 assistant_type;
    uint8 unused;
} APP_STAROT_WAKEUP_CONFIG_IND_T;
typedef APP_STAROT_WAKEUP_CONFIG_IND_T APP_STAROT_WAKEUP_CONFIG_IND;

typedef struct
{
    uint16 command;
    uint8 messageFrom;
    uint8 wear_enable;
    uint32 timestamp;
} APP_STAROT_WEAR_CONFIG_IND_T;
typedef APP_STAROT_WEAR_CONFIG_IND_T APP_STAROT_WEAR_CONFIG_IND;

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
enum {
    GAIA_COMMAND_STAROT_CALL_SETUP_BEGIN = 0X5005,                   // 通话接入 还没有接通
    GAIA_COMMAND_STAROT_CALL_SETUP_END =0x5006 ,
    GAIA_COMMAND_STAROT_CALL_AUDIO_IND = 0x5007,                        //
    GAIA_COMMAND_STAROT_CALL_AUDIO_CFM = 0x5008,                        // xxx
    GAIA_COMMAND_STAROT_CALL_AUDIO_END = 0x5009,                        // xxx
    GAIA_COMMAND_STAROT_CALL_SOURCE_IND = 0x500a,                       // xxx
    GAIA_COMMAND_STAROT_START_TRANS_AUDIO_IND = 0x500b,                 // xxx
    GAIA_COMMAND_STAROT_END_TRANS_AUDIO_IND = 0x500c,                   // xxx
    GAIA_COMMAND_STAROT_CALL_AUDIO_DEVICE = 0X500D,                     // xxx
    GAIA_COMMAND_STAROT_CALL_DEVICE_TRANS_AUDIO_REQUEST = 0X500E,       // xxx
    GAIA_COMMAND_STAROT_CALL_ATTR = 0X500F,                             // xxx
    GAIA_COMMAND_STAROT_CALL_ACTIVE = 0X5010,                           // 电话接通
    GAIA_COMMAND_STAROT_CALL_INACTIVE = 0X5011,                         // 电话结束
};


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
    GAIA_COMMAND_STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB = 0X5506,             // App设置语言唤醒是否使能
    GAIA_COMMAND_STAROT_BASE_INFO_GET_APOLLO_WAKEUP_ENB = 0X5507,             // App获取语言唤醒是否使能
    GAIA_COMMAND_STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB = 0X5508,               // App设置佩戴检测是否使能
    GAIA_COMMAND_STAROT_BASE_INFO_GET_ADORN_CHEAK_ENB = 0X5509,               // App获取佩戴检测是否使能
    GAIA_COMMAND_STAROT_BASE_INFO_ACTIVE_DISCONNECT = 0X550A,                 // 设备主动断开连接
    GAIA_COMMAND_STAROT_BASE_INFO_GET_SN = 0X550B,                            // 获取SN信息
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
    GAIA_CONNECT_STAROT_RECORD_STOP_REPORT = 0X5207,                          // 强制停止录音                             // 升级盒子固件
};

/////////////////////////////测试与生产///////////////////////////////
enum {
    GAIA_COMMAND_STAROT_TEST_PRODUCT_REST = 0X5600,                           // 恢复出厂设置
    GAIA_COMMAND_STAROT_TEST_APOLLO_STATUS = 0X5601,                          // 读取apollo状态
    GAIA_COMMAND_STAROT_TEST_ONLINE_DBG = 0X5602
};

/////////////////////////////固件升级使用///////////////////////////////
enum {
    GAIA_CONNECT_STAROT_UPDATE_FIRMWARE     = 0X5800,                             //固件升级使用
    GAIA_CONNECT_STAROT_UPDATE_FIRMWARE_MD5 = 0X5801,                             //回复固件的校验码
};
/////////////////////////////升级///////////////////////////////
enum {
    GAIA_COMMAND_STAROT_UPGRADE_ENTER_DFU = 0X5700,                           // 进入升级模式
    GAIA_COMMAND_STAROT_UPGRADE_EXIT_DFU = 0X5701,                            // 退出升级模式
    GAIA_COMMAND_STAROT_UPGRADE_GET_CURRENT_VERSION_DETAIL = 0X5702,          // 获取升级的详细信息
    GAIA_COMMAND_STAROT_UPGRADE_NOTIFY_COMMIT_STATUS = 0X5703,                // 通知升级提交的信息
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

/////////////////////////////////////////////初始化与销毁/////////////////////////////////////////
void starotGaiaInit(void);
void starotGaiaReset(void);
////////////////////////////////////////////////////////////////////////////////////////////////

bool starotGaiaHandleCommand(GAIA_STAROT_IND_T *message);
bool starotGaiaHandleData(GAIA_STAROT_IND_T *message);
bool starotGaiaHandleDataMD5(GAIA_STAROT_IND_T *message);
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
    STAROT_DIALOG_CALL_NUMBER,							  // ui -> gaia 当前通话的电话号码
    STAROT_AI_USER_START_RECORD,                          // ui -> (ui & dsp) AI请求录音
    STAROT_AI_USER_STOP_RECORD,                           // ui -> (ui & dsp) AI停止录音
    STAROT_RECORD_STOP_STATUS_REPORT,                     // dsp -> ui 上报停止录音状态
    STAROT_RECORD_CALLIN_STOP_STATUS_REPORT,              // ui -> ui 拨打上报停止录音状态
    STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT,             // ui -> ui 接听上报停止录音状态
    STAROT_RECORD_RETURN_THREE_POWER,                     // gaia -> ui App主动获取电量状态
    STAROT_NOTIFY_STATUS,                                 // ui -> gaia 状态发生变化
    STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB,               // gaia -> ui App设置语言唤醒是否使能
    STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB,                 // gaia -> ui App设置是否佩戴使能
    STAROT_APP_CONTROL_PREVIOUS_TRACK,                    // gaia -> ui App控制上一首
    STAROT_APP_CONTROL_NEXT_TRACK,                        // gaia -> ui App控制下一首
    STAROT_APP_NOTIFY_PEER_UPGRADE_ENTER_CFM,             // sync -> gaia 通知进入升级状态结果
    STAROT_APP_NOTIFY_PEER_UPGRADE_EXIT_CFM,              // sync -> gaia 通知进入升级状态结果
    STAROT_UI_NOTIFY_COMMIT_STATUS,                       // ui -> gaia 升级提交的状态
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

void gaiaNotifyAudioAcceptStatus(Task task, int command);

bool appGaiaIsConnectBySpp(void);


/*
 * 更新固件使用的数据结构
 */
typedef struct
{
    uint16 command;
    /* 头部信息 */
    uint16 data_length;/* 数据长度 */
    uint8 flag;
    uint8 type;
    uint8 sessionid;
    uint8 index;
    /* 数据 */
    uint8 data[4];
} GAIA_STAROT_DATA_T;


typedef struct {
    uint16 status;
} STAROT_DIALOG_STATUS_T ;

typedef struct {
    uint16 len;
    uint8 number[2];
} STAROT_DIALOG_CALL_NUMBER_T;

#endif // AV_HEADSET_GAIA_STAROT_H
