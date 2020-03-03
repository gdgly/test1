#ifndef SUB_UI_H
#define SUB_UI_H

void appSubUiHandleMessage(Task task, MessageId id, Message message);

#define GAIA_PAYLOAD_LEN        (28)       // 消息最大长度
#define MAKE_GAIA_MESSAGE_WITH_LEN(TYPE, LEN) \
    TYPE##_T *message = (TYPE##_T *) PanicUnlessMalloc(sizeof(TYPE##_T) + LEN);

#define MAKE_CALL_2_GAIA_MESSAGE(TYPE) \
    TYPE##_T *message = (TYPE##_T *) PanicUnlessMalloc(sizeof(TYPE##_T));

// 盒子发送相关的命令操作
#define APP_CASE_REPORT_VERSION    (2000)           // 盒子硬件版本信息等
#define APP_CASE_REPORT_INFO       (2001)           // 盒子报告当前信息
#define APP_CASE_SET_BLEINFO       (2005)           // 设置BLE信息
#define APP_CASE_SET_BTINFO        (2006)           // 盒子设置耳机经典蓝牙配对地址

#define APP_CHARGE_STATUS          (2010)           // 充电信息报告
#define APP_BTN_DOUBLE_TAP         (2011)           // 双击敲击
#define APP_PSENSOR_INEAR          (2012)           // 入耳
#define APP_PSENSOR_OUTEAR         (2013)           // 出耳机
#define APP_CONNECTED_HOST         (2014)           // App延时连接主耳机

#define APP_CALLIN_ACT             (2020)           // 拨号相关信息 拨入
#define APP_CALLIN_INACT           (2021)           // 拨号相关信息 拨入断开
#define APP_CALLOUT_ACT            (2022)           // 拨号相关信息 拨出
#define APP_CALLOUT_INACT          (2023)           // 拨号相关信息 拨出断开
#define APP_CALL_ACTIVE            (2024)           // 拨号相关信息 接听
#define APP_CALL_INACTIVE          (2025)           // 拨号相关信息 断开
#define APP_ASSISTANT_AWAKEN       (2026)           // apollo启动语音助手
#define APP_ASSISTANT_TAP_AWAKEN   (2027)           // 敲击启动语音助手
#define APP_ASSISTANT_TAP_SYSTEM   (2028)           // 敲击启动系统助手
#define APP_ASSISTANT_SYSTEM       (2029)           // apollo启动系统助手

#define APP_INTERNAL_HANDSET_PAIR  (2090)           // 内部消息，启动广播连接手机
#define APP_INTERNAL_POWERSAVECHG  (2091)           // 当前模式变化
#define APP_NOTIFY_DEVICE_CON_POS   2092            // rule/gaia -> ui 让ui主动上报位置、状态数据

#define APP_THREE_POWER            (2099)           // 废弃 ui -> ui 电量状态

#define APP_ATTACH_PLC_IN           (2100)           // 20340 -> ui 报告plc  in 放入充电盒
#define APP_ATTACH_PLC_OUT          (2101)           // 20340 -> ui 报告plc  out 取出充电盒
#define APP_PAIR_HEADSET            (2102)           // 20340 -> ui 报告plc  配对手机
#define APP_RESET_FACTORY           (2103)           // 20340 -> ui 报告plc 恢复出厂设置
#define APP_CASE_OPEN               (2104)           // 20340 -> ui 报告plc 充电盒打开
#define APP_CASE_CLOSE              (2105)           // 20340 -> ui 报告plc 充电盒关闭
#define APP_CASE_CLOSE_LATER        (2106)           // ui -> ui 延迟发送关闭规则
#define APP_HFP_CHANGE_AUDIO_DIRECT_TIMEOUT (2107)   // ui -> ui 空处理
#define APP_UPGRADE_APPLY_IND        (2108)          // upgrade -> ui 升级基本工作已经完毕，等待重启
#define APP_CHECK_VERSION            (2109)          //  sync -> ui 版本同步成功之后，检查版本
#define APP_UPGRADE_REBOOT_TIMEOUT   (2111)          //  ui -> ui 升级中，确定了对方版本和当前耳机一致，在同步版本之后，添加定时器，防止版本同步失败不能重启
#define APP_CHECK_PEER_FOR_UPDATE     2112           // upgrade/peersync -> ui 重启之后，如果是host continue状态，需要查询另一只耳机版本及其状态信息
#define APP_UPGRADE_COMMIT            2113           // peersync -> ui 比较版本之后，可以提交commit
#define APP_UPGRADE_ENTER_BY_PEER     2114           // peersync -> ui Peer通知当前耳机，进入升级模式
#define APP_UPGRADE_EXIT_BY_PEER      2115           // peersync -> ui Peer通知当前耳机，退出升级模式
#define APP_UPGRADE_ENTER_BY_GAIA     2116           // gaia -> ui Gaia通知当前耳机，进入升级模式
#define APP_UPGRADE_EXIT_BY_GAIA      2117           // gaia -> ui Gaia通知当前耳机，退出升级模式
#define APP_UPGRADE_COMMIT_STATUS               2118 // upgrade -> ui 通知升级状态
#define APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT       2119 // ui -> ui 定时返送，防止gaia没有连接，如果连接上了，不能收到升级状态
#define APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT_GRADE 2120 // ui -> ui 如果到了这个时间，还没有发送成功，取消通知
#define APP_UPGRADE_COPY_STATUS_GRADE 2121  // upgrade/ui -> ui 拷贝成功之后，再去更新临时版本，copy操作很耗时，10s以上，导致不同步问题

struct UI_APP_UPGRADE_COMMIT_STATUS_T {
    bool status;   // true:commit ;false:revert
};
typedef struct UI_APP_UPGRADE_COMMIT_STATUS_T UI_APP_UPGRADE_COMMIT_STATUS;

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

typedef struct {
	uint32 command;
} CALL_INDICATOR_T;

typedef struct {
        uint8_t number[25];
	uint8_t length;
} CALL_NUMBER_T;

enum {
    DIAL_IN_ACTIVE      = 1 << 0,
    DIAL_OUT_ACTIVE     = 1 << 1,
    DIAL_ACTIVE         = 1 << 2,
    DIAL_INACTIVE       = 1 << 3,
    DIAL_IN_INACTIVE    = 1 << 4,
    DIAL_OUT_INACTIVE   = 1 << 5
};
	
enum {CHARGE_ST_NONE=0, CHARGE_ST_CONNECT,
      CHARGE_ST_OK, CHARGE_ST_LOW,
      CHARGE_ST_FIN};
typedef enum {POWER_MODE_IN_CASE=1, POWER_MODE_IN_CASE_OPEN,
              POWER_MODE_OUT_CASE, POWER_MODE_IN_EAR}PowerSaveMode;    // powersave

enum {UI_CASE_CLOSE = 0, UI_CASE_OPEN = 1};
// 系统运行中的一些动态数据
typedef struct tagPROGRUNINFO {
    bdaddr         addr;                    // 本机蓝牙地址

    uint16         caseHWver, caseSWver;    // 盒子的版本信息
    uint16         caseElectrity:7;         // 盒子电量 0....100
    uint16         caseLidOpen:1;           // 盒子盖状态 0:close 1:opened
    uint16         caseKeyDown:1;           // 按键按下   0:up 1:down
    uint16         caseKeyLong:1;           // 按键按下长按键
    uint16         caseUsb:1;               // 盒子USB充电是否接上
    uint16         stopBle:1;               // BLE更新广播内容时，需要先停止广播

    uint16         peerGaiaStat:1;          // 对方手机GAIA与手机是否连接0：disconect, 1:connect
    uint16         gaiaStat:1;              // GAIA与手机是否连接0：disconect, 1:connect
    uint16         bredrconnect:1;           // 与经典蓝牙建立连接0：disconect, 1:connect
    uint16         peerconnect:1;            // 与对方耳机建立连接0：disconect, 1:connect
    uint16         handsetPair:2;           // 与手机配对 0：unknown, 1:success 2: fail
    uint16         recStat:1;               // 当前是否正在录音
    uint16         apolloWakeup:1;          // 当前apollo是否启动0:未启动, 1:启动
    uint16         chargeStat:3;            // 当前充电状态 CHARGE_ST_XXX

    uint8          peerPlace:3;             // 对方耳机状态 Bit0:盒子中 Bit1：空中 Bit2：佩戴
    uint8          peerElectrity;           // 对方耳机电量 0...100    
    uint8          iElectrity;              // 自己耳机电量 0...100

    bool           powerflag15;             // 15电量标记
    uint8          iWakeupTimes;            // 短时间记录 APO 唤醒次数

    PowerSaveMode  iPowerSaveMode;          // 当前模式

    uint8          dial_stat;               // 当前拨号情况
    uint8          dial_type:1;             // 1: 电话， 0：微信等其它
    //保存手机拨入的信息
    uint8          callIndex:2;
    CallIPtr       callInfo[MAX_CALLIN_INFO]; // 保存两个信息
#ifdef TWS_DEBUG
    bool           realInCase;              // true:充电盒中 false:空中
#endif
    uint8          currVer[DEV_HWSWVER_LEN];        // 对方耳机版本信息
    uint8          peerVer[DEV_HWSWVER_LEN];        // 对方耳机版本信息
    uint8          tempCurrentVer[DEV_SWVER_LEN];   // Gaia设置的临时当前耳机版本，当版本文件校验完毕之后更新系统的版本信息
    uint8          peerVerSyncStatus;               // 对方耳机版本信息同步状态
    bool           upgradeNeedReboot;               // 升级需要重启
    bool           canContinueUpgrade;              // 是否持续进入升级状态
    uint8          peerSnSyncStatus;                // SN同步状态
    uint8          peerSn[DEV_SN_LEN];              // 另一只耳机SN信息
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
/* EDR 配对成功与否 */
void appUiPairingComplete(void);
/* Play pairing failed prompt */
void appUiPairingFailed(void);
///////////////////////////////////////////////////////////////////////////////
///  APOLLO
///////////////////////////////////////////////////////////////////////////////
int apolloWakeupCallback(void);
bool appKymeraApolloIsRun(void);
void apolloWakeupPower(int enable);        // 开启或停止 APO2

///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
void appUiCaseStatus2(int16 stat, int16 power);           // 当前USB状态
void appUiCaseStatus2FromPeer(uint8 *buff);
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
bool appGetCaseIsOpen(void);


///////////////////////////////////////////////////////////////////////////////
///  Peer耳机版本操作
///  启动的时候，状态为空，发送之后，添加状态PeerVersionSyncStatusSent
///////////////////////////////////////////////////////////////////////////////
enum {
    PeerVersionSyncStatusSent = 1 << 0,
    PeerVersionSyncStatusRecv = 1 << 1,
    PeerVersionSyncStatusComplete = PeerVersionSyncStatusSent | PeerVersionSyncStatusRecv,
};
bool appPeerVersionSyncStatusIsComplete(void);
bool appPeerVersionSyncStatusHaveSent(void);
void appPeerVersionSyncStatusSet(uint8 status);  ///如果发生disconnect peer，重置状态
void appPeerVersionSyncStatusClean(uint8 status); /// 清楚指定状态的同步信息
void appPeerVersionSyncSent(void);
void appPeerVersionClearCache(void);

bool appPeerVersionIsEmpty(void);
void appPeerVersionSet(uint8* buffer);
uint8* appPeerVersionGet(void);
uint8* appCurrVersionGet(void);
void appUITempSetVersionToMemory(uint8* ptr);
void appUIConvertTempVersionToMemory(void);

bool appUICanContinueUpgrade(void);
/// 取消发送升级状态的定时器
void appUICancelAllUpgradeTime(void);

// region SN同步

void appUISetPeerSnDetail(uint8* peerSn, uint8 len);
void appUIClearPeerSnStatus(void);
void appUISetPeerSnStatus(uint8 status);
void appUIUnSetPeerSnStatus(uint8 status);
bool appUIGetPeerSnStatusIsComplete(void);
bool appUIGetPeerSnStatusIsHaveSent(void);
const uint8* appUIGetPeerSnDetail(void);

// endregion

#endif // SUB_UI_H
