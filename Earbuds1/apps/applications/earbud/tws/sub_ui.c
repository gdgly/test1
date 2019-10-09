#ifdef CONFIG_STAROT
#include <panic.h>
#include <ps.h>
#include <boot.h>
#include <pmalloc.h>
#include <input_event_manager.h>

#include "av_headset.h"
#include "av_headset_ui.h"
#include "av_headset_sm.h"
#include "av_headset_hfp.h"
#include "av_headset_power.h"
#include "av_headset_log.h"
#include "sub_ui.h"
#include "av_headset_gaia_starot.h"


ProgRunInfo gProgRunInfo;

/* BLE 已经连接到手机，则不需要修改广播内容 */
#define BLE_CONNECTED_PHONE()  (NULL != appGetGaiaTransport())
/* 如果APP没有初始化结束，很多消息不能处理 */
#define RETURN_APP_NOT_INIT()  do{if(appInitCompleted() == FALSE) return; }while(0)

/////////////////////////////////////////////////////////////////////////
///     向GAIA发送信息
/////////////////////////////////////////////////////////////////////////
// Task HFP 通过这个函数来发送消息给GAIA
// payload=[状态1 + 状态2 + 号码]
static int16 subUiCaller2Gaia(MessageId id, ProgRIPtr  progRun)
{
    uint8 count = 2;
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);

    (void)id;
    message->command      = STAROT_DIALOG_STATUS;
    message->payload[0]   = progRun->dial_stat;
    message->payload[1]   = 0;

    if(progRun->callIndex < MAX_CALLIN_INFO) {
        CallIPtr   pCall;

        pCall = progRun->callInfo[progRun->callIndex];
        if(pCall && (NULL != pCall->number)) {
            memcpy(&message->payload[2], pCall->number, pCall->size_number-1);
            count += pCall->size_number-1;
        }
    }
    message->payloadLen    = count;

    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);

    DEBUG_LOG("\nHFP CALL Status=0x%x LEN=%d", progRun->dial_stat, count);
    return 0;
}

//盒子状态的信息
// payload=[盒盖[1]+ 按键[1] + 长按键[1] + 电量[]]
static int16 subUiCasestat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    (void)id;
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);

    message->command = STAROT_DIALOG_CASE_VER;
    message->payload[0] = (uint8)progRun->caseLidOpen;
    message->payload[1] = (uint8)progRun->caseKeyDown;
    message->payload[2] = (uint8)progRun->caseKeyLong;
    message->payload[3] = (uint8)progRun->caseElectrity;

    message->payloadLen = 4;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
    return 0;
}

//盒子版本的信息
// payload=[HwVer[2] + SwVer[2]]
static int16 subUiCasever2Gaia(MessageId id, ProgRIPtr  progRun)
{
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);
    (void)id;

    message->command = STAROT_DIALOG_CASE_STAT;
    memcpy(&message->payload[0], &progRun->caseHWver, 2);
    memcpy(&message->payload[2], &progRun->caseSWver, 2);
//    message->payload[0] = progRun->caseHWver;
//    message->payload[2] = progRun->caseSWver;
    message->payloadLen = 4;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
    return 0;
}

static int16 subUiChargeStat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    (void)id,(void)progRun;

    return 0;
}

/////////////////////////////////////////////////////////////////////////
///     接收GAIA信息并处理
/////////////////////////////////////////////////////////////////////////
static void subUiGaiaMessage(ProgRIPtr progRun, Message message)
{
    GAIA_STAROT_MESSAGE_T *ind = (GAIA_STAROT_MESSAGE_T*)message;

    (void)progRun;
    switch(ind->command) {
    case STAROT_DIALOG_USER_ACCEPT_RECORD:
        progRun->recStat  = 1;
        break;
    case STAROT_DIALOG_USER_REJECT_RECORD:
        progRun->recStat  = 0;
        break;
    }
}

// UI 自定义的相关消息
void appSubUiHandleMessage(Task task, MessageId id, Message message)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    (void)task, (void)message;

    switch(id) {
    case MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT:
        if(progRun->iElectrity == ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent)
            break;
        progRun->iElectrity = ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent;
        DEBUG_LOG("appSubUiHandleMessage iElectrity=%d", progRun->iElectrity);
        break;

    // 拨号、电话相关的消息
    case HFP_CALLER_ID_IND:
        subUiCaller2Gaia(id, progRun);
        break;
    case APP_CALLIN_ACT:           // 拨号相关信息 拨入
    case APP_CALLIN_INACT:         // 拨号相关信息 拨入断开
    case APP_CALLOUT_ACT:          // 拨号相关信息 拨出
    case APP_CALLOUT_INACT:        // 拨号相关信息 拨出断开
    case APP_CALL_ACTIVE:          // 拨号相关信息 接听
    case APP_CALL_INACTIVE:        // 拨号相关信息 断开
        subUiCaller2Gaia(id, progRun);
        break;

    case GAIA_STAROT_COMMAND_IND:           // GAIA 返回过来的消息
        subUiGaiaMessage(progRun, message);
        break;

    // 盒子发送相关的命令操作
    case APP_CASE_REPORT_VERSION:           // 盒子硬件版本信息等
        subUiCasever2Gaia(id, progRun);
        break;
    case APP_CASE_REPORT_INFO:              // 盒子报告当前信息
        subUiCasestat2Gaia(id, progRun);
        break;
    case APP_CASE_SET_BLEINFO:              // 设置BLE信息
    case APP_CASE_SET_BTINFO:               // 盒子设置耳机经典蓝牙配对地址
        break;

    case APP_CHARGE_STATUS:                 // 充电状态变化
        RETURN_APP_NOT_INIT();

        if(BLE_CONNECTED_PHONE())
            subUiChargeStat2Gaia(id, progRun);
        else
            appUiRestartBle();
        break;
    }
}

void appSubUIInit(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    batteryRegistrationForm battery_from;

    memset(progRun, 0, sizeof(ProgRunInfo));

    /* 获取底层的电量信息 */
    battery_from.task            = &appGetUi()->task;
    battery_from.representation  = battery_level_repres_percent;
    battery_from.hysteresis      = 1;
    appBatteryRegister(&battery_from);
}

ProgRIPtr appSubGetProgRun(void)
{
   return &gProgRunInfo;
}

bdaddr* SystemGetEarAddr(uint8 *addrbuf) //获取蓝牙地址
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(addrbuf) {
        addrbuf[0] = (progRun->addr.nap >> 8) & 0xFF;
        addrbuf[1] = (progRun->addr.nap & 0xFF);
        addrbuf[2] = progRun->addr.uap;
        addrbuf[3] = (progRun->addr.lap >> 16) & 0xFF;
        addrbuf[4] = (progRun->addr.lap >> 8) & 0xFF;
        addrbuf[5] = (progRun->addr.lap & 0xFF);
    }

    return &progRun->addr;
}

static bool HfpCallerIsSame(uint8 *number, uint16 size_number)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    CallIPtr   pCall;

    if(progRun->callIndex >= MAX_CALLIN_INFO)
        return FALSE;

    if(NULL == (pCall = progRun->callInfo[progRun->callIndex]))
        return FALSE;

    if(NULL == pCall->number)
        return FALSE;

    if(number && size_number > 0) {
        if(memcmp(pCall->number, number, size_number) == 0)
            return TRUE;
     }

    return FALSE;
}

// HFP TASK调用，新的号码拨入
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    CallIPtr   pCall;

    if(HfpCallerIsSame(number, size_number) == TRUE)
        return 0;

    progRun->callIndex += 1;
    if(progRun->callIndex >= MAX_CALLIN_INFO)
        progRun->callIndex = 0;

    if(NULL == progRun->callInfo[progRun->callIndex]) {
        progRun->callInfo[progRun->callIndex] = (CallIPtr)PanicUnlessMalloc(sizeof(CallInfo));
        memset(progRun->callInfo[progRun->callIndex], 0, sizeof(CallInfo));
    }

    pCall = progRun->callInfo[progRun->callIndex];
    if(number && size_number > 0) {
        size_number += 1;
        if(pCall->number && pCall->size_number < size_number)
            pfree(pCall->number), pCall->number = NULL;

        if(pCall->number == NULL)
            pCall->number = PanicUnlessMalloc(size_number);

        pCall->size_number = size_number;
        memcpy(pCall->number, number, size_number-1);
        pCall->number[size_number] = 0;
        DEBUG_LOG("Call[%d]Number[%d]: %c %c %c %c", progRun->callIndex, size_number,
            number[7], number[8], number[9], number[10]);
    }

    if(name && size_name > 0) {
        size_name += 1;
        if(pCall->name && pCall->size_name < size_name)
            pfree(pCall->name), pCall->name = NULL;

        if(pCall->name == NULL)
            pCall->name = PanicUnlessMalloc(size_name);

        pCall->size_name = size_name;
        memcpy(pCall->name, name, size_name-1);
        pCall->name[size_name] = 0;
        DEBUG_LOG("CallName[%d]: %c %c %c %c", size_name,
            name[0], name[1], name[2], name[3]);
    }

    // 通知一下UI
    MessageSend(&appGetUi()->task, HFP_CALLER_ID_IND, 0);
    return 0;
}

// Call in:
// appUiHfpCallIncomingActive--> 接通 appUiHfpCallIncomingInactive(0) -->appUiHfpCallActive -->appUiHfpCallInactive挂断
//                           --> 挂断 appUiHfpCallIncomingInactive(1)
// Call out:
//appUiHfpCallOutcomingActive--> 接通 appUiHfpCallOutcomingInactive(0)-->appUiHfpCallActive -->appUiHfpCallInactive挂断
//                           --> 挂断 appUiHfpCallOutcomingInactive(1)

/* Show HFP incoming call */
void appUiHfpCallIncomingActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->callIndex = MAX_CALLIN_INFO;  // 设置为无效值

    progRun->dial_stat  &= ~DIAL_ST_INACT;
    progRun->dial_stat  |= DIAL_ST_IN;

    MessageSend(&appGetUi()->task, APP_CALLIN_ACT, 0);
}

/* Show HFP outcoming call */
void appUiHfpCallOutcomingActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat  &= ~DIAL_ST_INACT;
    progRun->dial_stat  |= DIAL_ST_OUT;

    MessageSend(&appGetUi()->task, APP_CALLOUT_ACT, 0);
}

/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_IN | DIAL_ST_ACT);

    MessageSend(&appGetUi()->task, APP_CALLIN_INACT, 0);
}

/* Cancel HFP outcoming call */
void appUiHfpCallOutcomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_OUT | DIAL_ST_ACT);

    MessageSend(&appGetUi()->task, APP_CALLOUT_INACT, 0);
}

/* Show HFP call active */
void appUiHfpCallActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat  |= DIAL_ST_ACT;

    MessageSend(&appGetUi()->task, APP_CALL_ACTIVE, 0);
}

/* Show HFP call imactive */
void appUiHfpCallInactive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat  &= ~(DIAL_ST_IN | DIAL_ST_OUT | DIAL_ST_ACT);
    progRun->dial_stat  |= DIAL_ST_INACT;

    MessageSend(&appGetUi()->task, APP_CALL_INACTIVE, 0);
}


///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
// 数值小于0，表示不确定，不设置
void appUiCaseStatus(int16 lidOpen, int16 keyDown, int16 keyLong, int16 iElectrity)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(lidOpen >= 0)
        progRun->caseLidOpen = (1 == lidOpen) ? 1 : 0;

    if(keyDown >= 0)
        progRun->caseKeyDown = (1 == keyDown) ? 1 : 0;

    if(keyLong >= 0)
        progRun->caseKeyLong = (1 == keyLong) ? 1 : 0;

    if(iElectrity >= 0)
        progRun->caseElectrity = iElectrity;

    MessageSend(&appGetUi()->task, APP_CASE_REPORT_INFO, 0);
}

void appUiCaseVersion(uint16 hwVer, uint16 swVer)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->caseHWver = hwVer;
    progRun->caseSWver = swVer;

    MessageSend(&appGetUi()->task, APP_CASE_REPORT_VERSION, 0);
}

// 临时停止BLE广播，以便开始新的广播内容
bool appUiIsStopBle(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    return (progRun->stopBle == 1) ? TRUE : FALSE;
}

void appUiRestartBle(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(BLE_CONNECTED_PHONE()) {
        DEBUG_LOG("Ble Connected, restart none");
        return;
    }

    progRun->stopBle = 1;
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_BLE_CONNECTABLE_CHANGE);

    progRun->stopBle = 0;
}

///////////////////////////////////////////////////////////////////////////////
///  充电模块反馈的信息
///////////////////////////////////////////////////////////////////////////////
/// 变化流程为：appUiChargerConnected -->appUiChargerChargingOk(大电流） 循环消息
///                                   -->appUiChargerChargingLow         循环消息
///                                   -->appUiChargerComplete
///              appUiChargerDisconnected
void appUiChargerConnected(void)
{
    DEBUG_LOG("appUiChargerConnected");

    appSubGetProgRun()->chargeStat = CHARGE_ST_CONNECT;
    MessageSendLater(&appGetUi()->task, APP_CHARGE_STATUS, 0, 500);
}

void appUiChargerDisconnected(void)
{
    DEBUG_LOG("appUiChargerDisconnected");

    appSubGetProgRun()->chargeStat = CHARGE_ST_NONE;
    MessageSend(&appGetUi()->task, APP_CHARGE_STATUS, 0);
}

void appUiChargerChargingLow(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(progRun->chargeStat == CHARGE_ST_LOW)
        return;

    progRun->chargeStat = CHARGE_ST_LOW;
    DEBUG_LOG("appUiChargerChargingLow");
}

void appUiChargerChargingOk(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(progRun->chargeStat == CHARGE_ST_OK)
        return;

    progRun->chargeStat = CHARGE_ST_OK;
    DEBUG_LOG("appUiChargerChargingOk");
    MessageSendLater(&appGetUi()->task, APP_CHARGE_STATUS, 0, 500);
}

void appUiChargerComplete(void)
{
    DEBUG_LOG("appUiChargerComplete");

    appSubGetProgRun()->chargeStat = CHARGE_ST_FIN;
    MessageSendLater(&appGetUi()->task, APP_CHARGE_STATUS, 0, 500);
}

#endif
