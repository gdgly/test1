#ifndef SUB_UI_H
#define SUB_UI_H

void appSubUiHandleMessage(Task task, MessageId id, Message message);

#define GAIA_PAYLOAD_LEN        (28)       // 消息最大长度
#define MAKE_GAIA_MESSAGE_WITH_LEN(TYPE, LEN) \
    TYPE##_T *message = (TYPE##_T *) PanicUnlessMalloc(sizeof(TYPE##_T) + LEN);


// 盒子发送相关的命令操作
#define APP_CASE_REPORT_VERSION    (2000)           // 盒子硬件版本信息等
#define APP_CASE_REPORT_INFO       (2001)           // 盒子报告当前信息
#define APP_CASE_SET_BLEINFO       (2005)           // 设置BLE信息
#define APP_CASE_SET_BTINFO        (2006)           // 盒子设置耳机经典蓝牙配对地址

#define APP_CHARGE_STATUS          (2010)           // 充电信息报告
#define APP_BTN_DOUBLE_TAP         (2011)           // 双击敲击
#define APP_PSENSOR_INEAR          (2012)           // 入耳
#define APP_PSENSOR_OUTEAR         (2013)           // 出耳机

#define APP_CALLIN_ACT             (2020)           // 拨号相关信息 拨入
#define APP_CALLIN_INACT           (2021)           // 拨号相关信息 拨入断开
#define APP_CALLOUT_ACT            (2022)           // 拨号相关信息 拨出
#define APP_CALLOUT_INACT          (2023)           // 拨号相关信息 拨出断开
#define APP_CALL_ACTIVE            (2024)           // 拨号相关信息 接听
#define APP_CALL_INACTIVE          (2025)           // 拨号相关信息 断开
#define APP_ASSISTANT_AWAKEN       (2026)           // apollo启动语音助手
#define APP_ASSISTANT_TAP_AWAKEN   (2027)           // 敲击启动语音助手
#define APP_TAP_SYSTEM             (2028)           // 敲击启动系统助手

#define APP_INTERNAL_HANDSET_PAIR  (2090)           // 内部消息，启动广播连接手机
#define APP_INTERNAL_POWERSAVECHG  (2091)           // 当前模式变化

#define APP_THREE_POWER            (2099)           // ui -> ui 电量状态

#define APP_ATTACH_PLC_IN           (2100)           // 20340 -> ui 报告plc  in 放入充电盒
#define APP_ATTACH_PLC_OUT          (2101)           // 20340 -> ui 报告plc  out 取出充电盒
#define APP_PAIR_HEADSET            (2102)           // 20340 -> ui 报告plc  配对手机
#define APP_RESET_FACTORY           (2103)           // 20340 -> ui 报告plc 恢复出厂设置
#define APP_CASE_OPEN               (2104)           // 20340 -> ui 报告plc 充电盒打开
#define APP_CASE_CLOSE              (2105)           // 20340 -> ui 报告plc 充电盒关闭


//#define APP_CASE_GET_INFO          (2010)           // 获取版本信息
//#define APP_CASE_GET_BTINFO        (2011)           // 盒子获取耳机经典蓝牙地址
//#define APP_CASE_GET_BLEINFO       (2013)           // 获取BLE信息
//#define APP_CASE_GET_STATUS        (2015)           // 获取当前状态(充电满)

typedef struct tagCALLINFO {
    uint8         *number;
    uint16         size_number:15;
    uint16         income:1;

    uint8         *name;
    uint16         size_name;
}CallInfo, *CallIPtr;
#define MAX_CALLIN_INFO       (2)

/* 使用BIT位表示当前实时状态 */
enum {DIAL_ST_IN=1, DIAL_ST_OUT=2,                    // 电话拨入/出
      DIAL_ST_ACT=4, DIAL_ST_INACT=8,                 // 电话拨入接听中
     };
enum {CHARGE_ST_NONE=0, CHARGE_ST_CONNECT,
      CHARGE_ST_OK, CHARGE_ST_LOW,
      CHARGE_ST_FIN};
typedef enum {POWER_MODE_IN_CASE=1, POWER_MODE_IN_CASE_OPEN,
              POWER_MODE_OUT_CASE, POWER_MODE_IN_EAR}PowerSaveMode;    // powersave
// 系统运行中的一些动态数据
typedef struct tagPROGRUNINFO {
    bdaddr         addr;                    // 本机蓝牙地址

    uint16         caseHWver, caseSWver;    // 盒子的版本信息
    uint16         caseElectrity:7;         // 盒子电量 0....100
    uint16         caseLidOpen:1;           // 盒子盖状态 0:close 1:opened
    uint16         caseKeyDown:1;           // 按键按下   0:up 1:down
    uint16         caseKeyLong:1;           // 按键按下长按键
    uint16         stopBle:1;               // BLE更新广播内容时，需要先停止广播

    uint16         gaiaStat:1;              // GAIA与手机是否连接0：disconect, 1:connect
    uint16         bredrconnect:1;           // 与经典蓝牙建立连接0：disconect, 1:connect
    uint16         peerconnect:1;            // 与对方耳机建立连接0：disconect, 1:connect
    uint16         recStat:1;               // 当前是否正在录音
    uint16         apolloWakeup:1;          // 当前apollo是否启动0:未启动, 1:启动
    uint16         chargeStat:3;            // 当前充电状态 CHARGE_ST_XXX

    uint8          peerPlace:3;             // 对方耳机状态 Bit0:盒子中 Bit1：空中 Bit2：佩戴
    uint8          peerElectrity;           // 对方耳机电量 0...100    
    uint8          iElectrity;              // 自己耳机电量 0...100

    uint8          iWakeupTimes;            // 短时间记录 APO 唤醒次数

    PowerSaveMode  iPowerSaveMode;          // 当前模式

    uint8          dial_stat;               // 当前拨号情况
    uint8          dial_type:1;             // 1: 电话， 0：微信等其它
    //保存手机拨入的信息
    uint8          callIndex:2;
    CallIPtr       callInfo[MAX_CALLIN_INFO]; // 保存两个信息

    uint8          powerCaseState;          // 0 close 1 open
#ifdef TWS_DEBUG
    bool           realInCase;              // true:充电盒中 false:空中
#endif

}ProgRunInfo, *ProgRIPtr;
extern ProgRunInfo gProgRunInfo;
void appSubUIInit(void);
ProgRIPtr appSubGetProgRun(void);
bdaddr* SystemGetEarAddr(uint8 *addrbuf); //获取蓝牙地址

/* Get Call number */
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name);
/* Get Dial number */
int16 appUiHfpDialId(uint8 *number, uint16 size_number);
/* Show HFP incoming call */
void appUiHfpCallIncomingActive(void);
void appUiHfpCallOutcomingActive(void);
/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(int16 isEnd);
void appUiHfpCallOutcomingInactive(int16 isEnd);
/* Show HFP call active */
void appUiHfpCallActive(void);
/* Show HFP call imactive */
void appUiHfpCallInactive(void);
/*EDR connect state*/
void appUiAvConnected(unsigned cad);
/*EDR disconnect state*/
void appUiAvDisconnected(void);

///////////////////////////////////////////////////////////////////////////////
///  APOLLO
///////////////////////////////////////////////////////////////////////////////
int apolloWakeupCallback(void);
bool appKymeraApolloIsRun(void);
void apolloWakeupPower(int enable);        // 开启或停止 APO2

///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
// bitEars: BIT0/1:Left/Right Mask; Bit4/5 Left/Right in or Not
void appUiCaseStatus(int16 lidOpen, int16 keyDown, int16 keyLong, int16 iElectrity, uint16 bitEars);
void appUiCaseVersion(uint16 hwVer, uint16 swVer);
// 盒子发送过来对方耳机地址
void appUiCaseSetPeerBtAddr(uint8 *addrbuf);
bool appUiIsStopBle(void);
void appUiRestartBle(void);

void appUiPowerSave(PowerSaveMode mode);     // 省电模式
void appUiPowerSaveSync(void);

///////////////////////////////////////////////////////////////////////////////
///  充电模块反馈的信息
///////////////////////////////////////////////////////////////////////////////
void appUiChargerConnected(void);
void appUiChargerDisconnected(void);
void appUiChargerChargingLow(void);
void appUiChargerChargingOk(void);
void appUiChargerComplete(void);
uint8 appUiGetPower(void);          // 获取当前耳机电量

///////////////////////////////////////////////////////////////////////////////
///  耳机连接
///////////////////////////////////////////////////////////////////////////////
void appUIBudsPosition(int type);
void appUICaseEvent(int type);

///////////////////////////////////////////////////////////////////////////////
///  充电盒是否关闭/打开
///////////////////////////////////////////////////////////////////////////////
uint8 appUIGetPowerCaseState(void);

bool appUIDeviceRealInCase(void);

bool appUICaseIsOpen(void);

void appGetLocalBrEdrAddress(uint8* addrbuf);
void appGetPeerBrEdrAddress(uint8* addrbuf);
#endif // SUB_UI_H
