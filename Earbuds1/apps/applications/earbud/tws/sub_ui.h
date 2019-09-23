#ifndef SUB_UI_H
#define SUB_UI_H

void appSubUiHandleMessage(Task task, MessageId id, Message message);

// 盒子发送相关的命令操作
#define APP_CASE_REPORT_VERSION    (1100)           // 盒子硬件版本信息等
#define APP_CASE_REPORT_INFO       (1101)           // 盒子报告当前信息
#define APP_CASE_SET_BLEINFO       (1105)           // 设置BLE信息
#define APP_CASE_SET_BTINFO        (1106)           // 盒子设置耳机经典蓝牙配对地址
//#define APP_CASE_GET_INFO          (1110)           // 获取版本信息
//#define APP_CASE_GET_BTINFO        (1111)           // 盒子获取耳机经典蓝牙地址
//#define APP_CASE_GET_BLEINFO       (1113)           // 获取BLE信息
//#define APP_CASE_GET_STATUS        (1115)           // 获取当前状态(充电满)

typedef struct tagCALLINFO {
    uint8         *number;
    uint16         size_number;

    uint8         *name;
    uint16         size_name;
}CallInfo, *CallIPtr;
#define MAX_CALLIN_INFO       (2)


// 系统运行中的一些动态数据
typedef struct tagPROGRUNINFO {
    bdaddr         addr;                    // 本机蓝牙地址

    uint16         caseHWver, caseSWver;    // 盒子的版本信息
    uint16         caseElectrity:7;         // 盒子电量 0....100
    uint16         caseLidOpen:1;           // 盒子盖状态 0:close 1:opened
    uint16         caseKeyDown:1;           // 按键按下   0:up 1:down
    uint16         caseKeyLong:1;           // 按键按下长按键


    uint8          peerElectrity;           // 对方耳机电量 0...100

    //保存手机拨入的信息
    uint8          callIndex:2;
    CallIPtr       callInfo[MAX_CALLIN_INFO]; // 保存两个信息


}ProgRunInfo, *ProgRIPtr;
extern ProgRunInfo gProgRunInfo;

/* Get Call number */
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name);
/* Show HFP incoming call */
void appUiHfpCallIncomingActive(void);
/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(void);
/* Show HFP call active */
void appUiHfpCallActive(void);
/* Show HFP call imactive */
void appUiHfpCallInactive(void);


#endif // SUB_UI_H
