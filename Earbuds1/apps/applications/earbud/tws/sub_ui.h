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

#define APP_CALLIN_ACT             (2020)           // 拨号相关信息 拨入
#define APP_CALLIN_INACT           (2021)           // 拨号相关信息 拨入断开
#define APP_CALLOUT_ACT            (2022)           // 拨号相关信息 拨出
#define APP_CALLOUT_INACT          (2023)           // 拨号相关信息 拨出断开
#define APP_CALL_ACTIVE            (2024)           // 拨号相关信息 接听
#define APP_CALL_INACTIVE          (2025)           // 拨号相关信息 断开

//#define APP_CASE_GET_INFO          (2010)           // 获取版本信息
//#define APP_CASE_GET_BTINFO        (2011)           // 盒子获取耳机经典蓝牙地址
//#define APP_CASE_GET_BLEINFO       (2013)           // 获取BLE信息
//#define APP_CASE_GET_STATUS        (2015)           // 获取当前状态(充电满)

typedef struct tagCALLINFO {
    uint8         *number;
    uint16         size_number;

    uint8         *name;
    uint16         size_name;
}CallInfo, *CallIPtr;
#define MAX_CALLIN_INFO       (2)

/* 使用BIT位表示当前实时状态 */
enum {DIAL_ST_IN=1, DIAL_ST_OUT=2,                    // 电话拨入/出
      DIAL_ST_ACT=4, DIAL_ST_INACT=8,                 // 电话拨入接听中
     };

// 系统运行中的一些动态数据
typedef struct tagPROGRUNINFO {
    bdaddr         addr;                    // 本机蓝牙地址

    uint16         caseHWver, caseSWver;    // 盒子的版本信息
    uint16         caseElectrity:7;         // 盒子电量 0....100
    uint16         caseLidOpen:1;           // 盒子盖状态 0:close 1:opened
    uint16         caseKeyDown:1;           // 按键按下   0:up 1:down
    uint16         caseKeyLong:1;           // 按键按下长按键

    uint16         recStat:1;               // 当前是否正在录音


    uint8          peerElectrity;           // 对方耳机电量 0...100

    uint8          dial_stat;               // 当前拨号情况
    //保存手机拨入的信息
    uint8          callIndex:2;
    CallIPtr       callInfo[MAX_CALLIN_INFO]; // 保存两个信息


}ProgRunInfo, *ProgRIPtr;
extern ProgRunInfo gProgRunInfo;

/* Get Call number */
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name);
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

///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
void appUiCaseStatus(int16 lidOpen, int16 keyDown, int16 keyLong, int16 iElectrity);
void appUiCaseVersion(uint16 hwVer, uint16 swVer);

#endif // SUB_UI_H