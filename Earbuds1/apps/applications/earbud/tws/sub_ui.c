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

extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);
extern void disable_audio_forward(bool disable);
void HfpDialNumberRequest(hfp_link_priority priority, uint16 length, const uint8 *number);
void appUiBatteryStat(uint8 lbatt, uint8 rbatt, uint16 cbatt);

ProgRunInfo gProgRunInfo;

/* BLE 已经连接到手机，则不需要修改广播内容, 没有连接到手机则信息不需要发送出去 */
#define BLE_CONNECTED_PHONE()  (NULL != appGetGaiaTransport())
/* 如果APP没有初始化结束，很多消息不能处理 */
#define RETURN_APP_NOT_INIT()  do{if(appInitCompleted() == FALSE) return; }while(0)

static void subUiKeyDoubleTap(ProgRIPtr progRun)
{
    DEBUG_LOG("APP_MFB_BUTTON_PRESS %p", progRun);
    if (appSmIsOutOfCase())
    {
        /* If voice call active, hangup */
        if (appHfpIsCallActive())
            appHfpCallHangup();
            /* Sco Forward can be streaming a ring tone */
        else if (appScoFwdIsReceiving() && !appScoFwdIsCallIncoming())
            appScoFwdCallHangup();
        /* If outgoing voice call, hangup */
        else if (appHfpIsCallOutgoing())
            appHfpCallHangup();
        /* If incoming voice call, accept */
        else if (appHfpIsCallIncoming())
            appHfpCallAccept();
        else if (appScoFwdIsCallIncoming())
            appScoFwdCallAccept();
        /* If AVRCP to handset connected, send play or pause */
        else if (appDeviceIsHandsetAvrcpConnected())
            appAvPlayToggle(TRUE);
        /* If AVRCP is peer is connected and peer is connected to handset, send play or pause */
        else if (appDeviceIsPeerAvrcpConnectedForAv() && appPeerSyncIsComplete() && appPeerSyncIsPeerHandsetAvrcpConnected())
            appAvPlayToggle(TRUE);
        else if (appDeviceIsHandsetHfpConnected() && appDeviceIsHandsetA2dpConnected())
            appUiError();
        else if(!appSmIsPairing())
            appSmPairHandset();
        else
        {
            appSmConnectHandset();
            appUiAvConnect();
        }
    }
}

// 唤醒时是需要设置BIAS
static void appSubUISetMicbias(int set)
{
    uint16 value = (set ? MIC_BIAS_FORCE_ON : MIC_BIAS_OFF);

    MicbiasConfigure(MIC_BIAS_0, MIC_BIAS_VOLTAGE, appConfigMic0BiasVoltage());
    MicbiasConfigure(MIC_BIAS_0, MIC_BIAS_ENABLE, value);
}


/////////////////////////////////////////////////////////////////////////
///     向GAIA发送信息
/////////////////////////////////////////////////////////////////////////
// Task HFP 通过这个函数来发送消息给GAIA
// payload=[状态1 + 状态2 + 号码]
static int16 subUiCaller2Gaia(MessageId id, ProgRIPtr  progRun)
{
    uint8 count = 2;

    if(1 != progRun->gaiaStat)
        return -1;

    if(id == APP_CALLIN_INACT )
        return -1;

    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);

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

    DEBUG_LOG("HFP CALL Status=0x%x LEN=%d", progRun->dial_stat, count);
    return 0;
}

// Task HFP 通过这个函数来发送消息给GAIA,报告当前为语音通话还是WX等其它
// payload=[状态1] 1：语音通话，0：其它
static int16 subUiCallType2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 2);

    (void)id;
    message->command      = STAROT_DIALOG_TYPE;
    message->payload[0]   = progRun->dial_type;
    message->payloadLen   = 1;

    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);

    DEBUG_LOG("Audio for Status=%d", progRun->dial_type);
    return 0;
}

//盒子状态的信息
// payload=[盒盖[1]+ 按键[1] + 长按键[1] + 电量[]]
static int16 subUiCasestat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    (void)id;
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 4);

    message->command = STAROT_DIALOG_CASE_STAT;
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
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 4);
    (void)id;

    message->command = STAROT_DIALOG_CASE_VER;
    memcpy(&message->payload[0], &progRun->caseHWver, 2);
    memcpy(&message->payload[2], &progRun->caseSWver, 2);
    message->payloadLen = 4;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
    return 0;
}

//上报stop停止状态
static void subUiStopReport2Gaia(void)
{
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 1);

    message->command = GAIA_CONNECT_STAROT_RECORD_STOP_REPORT;
    message->payload[0] = 0X01;
    message->payloadLen = 1;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
}

static int16 subUiStat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    (void)id;
    phyState state = appPhyStateGetState();
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 5);

    message->command = GAIA_COMMAND_STAROT_BASE_INFO_NOTIFY_POWER_POSITION_CONNECTION;
    message->payload[2] = (uint8)progRun->caseElectrity;
    message->payload[3] = 0x00;
    message->payload[4] = 0X00;
    if(appConfigIsLeft()){
        message->payload[0] = (uint8)progRun->iElectrity;
        message->payload[1] = (uint8)progRun->peerElectrity;
        if((uint8)progRun->gaiaStat)
            message->payload[4] |= 0X80;
        if((uint8)progRun->bredrconnect)
            message->payload[4] |= 0X40;
        if((uint8)progRun->peerconnect)
            message->payload[4] |= 0X20;
        switch(state) {
        case PHY_STATE_IN_CASE:
            message->payload[3] |= 0x80;
            break;
        case PHY_STATE_OUT_OF_EAR:
        case PHY_STATE_OUT_OF_EAR_AT_REST:
            message->payload[3] |= 0x40;
            break;
        case PHY_STATE_IN_EAR:
            message->payload[3] |= 0x20;
            break;
        case PHY_STATE_UNKNOWN:
            break;
        }
        message->payload[3] |= (progRun->peerPlace >> 3);
        message->payload[4] |= ((message->payload[4] & 0XE0) >> 3);

    }
    else {
        message->payload[0] = (uint8)progRun->peerElectrity;
        message->payload[1] = (uint8)progRun->iElectrity;
        if((uint8)progRun->gaiaStat)
            message->payload[4] |= 0X10;
        if((uint8)progRun->bredrconnect)
            message->payload[4] |= 0X08;
        if((uint8)progRun->peerconnect)
            message->payload[4] |= 0X04;
        switch(state) {
        case PHY_STATE_IN_CASE:
            message->payload[3] |= 0x10;
            break;
        case PHY_STATE_OUT_OF_EAR:
        case PHY_STATE_OUT_OF_EAR_AT_REST:
            message->payload[3] |= 0x08;
            break;
        case PHY_STATE_IN_EAR:
            message->payload[3] |= 0x04;
            break;
        case PHY_STATE_UNKNOWN:
            break;
        }
        message->payload[3] |= /*progRun->peerPlace*/((message->payload[3] & 0X1C) << 3);
        message->payload[4] |= ((message->payload[4] & 0X1C) << 3);
    }
    message->payloadLen = 5;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);

    return 0;
}

/////////////////////////////////////////////////////////////////////////
///     接收GAIA信息并处理
/////////////////////////////////////////////////////////////////////////
static void subUiGaiaMessage(ProgRIPtr progRun, Message message)
{
    GAIA_STAROT_IND_T *ind = (GAIA_STAROT_MESSAGE_T*)message;
    (void)progRun;
    switch(ind->command) {
    case GAIA_COMMAND_STAROT_CONTROL_CALL_DIALOG:        ///调用拨打电话函数
        HfpDialNumberRequest(hfp_primary_link, ind->payloadLen, ind->payload);
        break;
    case GAIA_COMMAND_STAROT_CONTROL_ACCEPT_DIALOG:      ///调用接听电话
        appHfpCallAccept();
        break;
    case GAIA_COMMAND_STAROT_CONTROL_REJECT_DIALOG:      ///调用拒接电话
        appHfpCallHangup();
        break;

    case STAROT_AI_USER_START_RECORD:               ///设备开始录音
        progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
        appKymeraRecordStart();
        disable_audio_forward(FALSE);
#endif
        break;

    case STAROT_AI_USER_STOP_RECORD:               ///设备停止录音
        progRun->recStat  = 0;
#ifdef CONFIG_REC_ASSISTANT
        disable_audio_forward(TRUE);
        appKymeraRecordStop();
#endif
        break;

    case STAROT_DIALOG_USER_ACCEPT_RECORD:
        disable_audio_forward(FALSE);
        break;

    case STAROT_DIALOG_USER_REJECT_RECORD:
        disable_audio_forward(TRUE);
        break;
    case STAROT_RECORD_RETURN_THREE_POWER:
        subUiStat2Gaia(ind->command, progRun);
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
        subUiStat2Gaia(id, progRun);
        DEBUG_LOG("appSubUiHandleMessage iElectrity=%d", progRun->iElectrity);
        break;

    // GAIA appGaiaClientRegister NOTIFY 过来的消息
    case APP_GAIA_UPGRADE_ACTIVITY:
    case APP_GAIA_UPGRADE_CONNECTED:
    case APP_GAIA_UPGRADE_DISCONNECTED:
        break;
    case APP_GAIA_CONNECTED:
        DEBUG_LOG("GAIA connect to phone");
        progRun->gaiaStat  = 1;
        break;
    case APP_GAIA_DISCONNECTED:
        DEBUG_LOG("GAIA disconnect from phone");
        progRun->gaiaStat  = 0;
        break;

    case APP_BTN_DOUBLE_TAP:
        subUiKeyDoubleTap(progRun);
        break;

    case INIT_CFM:
        DEBUG_LOG("appSubUiHandleMessage INIT_CFM start");
        appSubUISetMicbias(TRUE);
        appGaiaClientRegister(appGetUiTask());                         // 获取GAIA的连接断开消息
        MessageSendLater(task, APP_INTERNAL_HANDSET_PAIR, NULL, 500);  // 启动广播(没有手机连接时)
        break;
    case APP_INTERNAL_HANDSET_PAIR:
        if(ConnectionTrustedDeviceListSize() > 0)
            break;

        if(appSmStateIsIdle(appGetState()))
            appSmPairHandset();
        else
            MessageSendLater(task, APP_INTERNAL_HANDSET_PAIR, NULL, 500);
        break;

    // 拨号、电话相关的消息
    case HFP_CALLER_ID_IND:
        subUiCaller2Gaia(id, progRun);
        subUiCallType2Gaia(id, progRun);
        break;
    case HFP_CURRENT_CALLS_IND:
        subUiCaller2Gaia(id, progRun);
        subUiCallType2Gaia(id, progRun);
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
    case APP_THREE_POWER:         // 电量变化
        subUiStat2Gaia(id, progRun);
        break;
    case APP_CHARGE_STATUS:                 // 充电状态变化
        RETURN_APP_NOT_INIT();

        if(BLE_CONNECTED_PHONE())
            subUiStat2Gaia(id, progRun);
        else
            appUiRestartBle();
        break;
    case STAROT_RECORD_STOP_STATUS_REPORT:
        subUiStopReport2Gaia();
        break;
     default:
        DEBUG_LOG("Unknown Message id=0x%x", id);
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

static bool HfpCallerIsSame(uint8 *number, uint16 size_number, uint16 income)
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
        if(memcmp(pCall->number, number, size_number) == 0 && pCall->income == income)
            return TRUE;
     }

    return FALSE;
}

static int16 appUiHfpSaveId(uint8 *number, uint16 size_number,
                            uint8 *name, uint16 size_name, uint16 income)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    CallIPtr   pCall;

    if(HfpCallerIsSame(number, size_number, income) == TRUE)
        return -1;

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
        pCall->income      = income;
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

    return 0;
}

// 电话语音与WX语音区分
// 获取到来电电话号码，语音通话
// 获取到拨出电话号码信息，并且长度>1 语音通话
// 获取到拨出电话号码信息，并且长度==0  WX等其它通话

// HFP TASK调用，新的号码拨入
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name)
{
    if(appUiHfpSaveId(number, size_number, name, size_name, 1) < 0)
        return 0;       // 重复信息

    appSubGetProgRun()->dial_type = 1;

    // 通知一下UI
    MessageSend(&appGetUi()->task, HFP_CALLER_ID_IND, 0);
    return 0;
}

//获取拨出的号码
int16 appUiHfpDialId(uint8 *number, uint16 size_number)
{
    if(0 == size_number)
        appSubGetProgRun()->dial_type = 0;
    else {
        appSubGetProgRun()->dial_type = 1;
        appUiHfpSaveId(number, size_number, NULL, 0, 0);
    }

    MessageSend(&appGetUi()->task, HFP_CURRENT_CALLS_IND, 0);
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

/*EDR connect state*/
void appUiAvConnected(unsigned cad)
{
    (void)cad;
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->bredrconnect = 1;

    MessageSend(&appGetUi()->task, APP_THREE_POWER, 0);
}

/*EDR disconnect state*/
void appUiAvDisconnected(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->bredrconnect = 0;

    MessageSend(&appGetUi()->task, APP_THREE_POWER, 0);
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
        appUiBatteryStat(-1, -1, iElectrity);

    MessageSend(&appGetUi()->task, APP_CASE_REPORT_INFO, 0);
}

void appUiCaseVersion(uint16 hwVer, uint16 swVer)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->caseHWver = hwVer;
    progRun->caseSWver = swVer;

    MessageSend(&appGetUi()->task, APP_CASE_REPORT_VERSION, 0);
}

void appUiBatteryStat(uint8 lbatt, uint8 rbatt, uint16 cbatt)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(lbatt >= 0)
        progRun->iElectrity = lbatt;
    if(rbatt >= 0)
        progRun->peerElectrity = rbatt;
    if(cbatt >= 0)
        progRun->caseElectrity = cbatt;

    MessageSend(&appGetUi()->task, APP_THREE_POWER, 0);
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
