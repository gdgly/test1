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
#include "apollo.h"

extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);
extern void disable_audio_forward(bool disable);
extern bool max20340_GetConnect(void);
void HfpDialNumberRequest(hfp_link_priority priority, uint16 length, const uint8 *number);
void appUiBatteryStat(uint8 lbatt, uint8 rbatt, uint16 cbatt);
void appSubUISetMicbias(int set);

ProgRunInfo gProgRunInfo;
uint8 g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;      // 设置为 NO_MIC，就是不使用这个MIC（使用单MIC）

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
#ifndef TWS_DEBUG
        else if(!appSmIsPairing())
            appSmPairHandset();
#endif
        else
        {
            appSmConnectHandset();
            appUiAvConnect();
        }
    }
}

// 唤醒时是需要设置BIAS
void appSubUISetMicbias(int set)
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
    (void)id;

//    if(id == APP_CALLIN_INACT )
//        return -1;

    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);

    message->command      = STAROT_DIALOG_STATUS;
    message->payload[0]   = progRun->dial_stat;
    message->payload[1]   = 0;

    if(progRun->callIndex < MAX_CALLIN_INFO) {
        CallIPtr   pCall;

        pCall = progRun->callInfo[progRun->callIndex];
        if(pCall && (NULL != pCall->number)) {
            memcpy(&message->payload[2], pCall->number, pCall->size_number);
            count += pCall->size_number;
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
    if(1 != progRun->gaiaStat)
        return -1;

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
    if(1 != progRun->gaiaStat)
        return -1;

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
static void subUiStopReport2Gaia(MessageId id)
{
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 1);

    message->command = GAIA_CONNECT_STAROT_RECORD_STOP_REPORT;
    if(STAROT_RECORD_STOP_STATUS_REPORT == id)
        message->payload[0] = 0X01;
    if(STAROT_RECORD_CALLIN_STOP_STATUS_REPORT == id)
        message->payload[0] = 0X02;
    if(STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT == id)
        message->payload[0] = 0X03;
    message->payloadLen = 1;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
}

static int16 subUiStartAssistant2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 4);
    (void)id;

    message->command = GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START;
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
    return 0;
}

static int16 subUiStat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    (void)id;
    phyState state = appPhyStateGetState();
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 5);

    message->command = STAROT_NOTIFY_STATUS;
    message->payload[2] = (uint8)progRun->caseElectrity;
    message->payload[3] = 0X00;
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
            message->payload[3] |= 0X80;
            break;
        case PHY_STATE_OUT_OF_EAR:
        case PHY_STATE_OUT_OF_EAR_AT_REST:
            message->payload[3] |= 0X40;
            break;
        case PHY_STATE_IN_EAR:
            message->payload[3] |= 0X20;
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
        message->payload[3] |= progRun->peerPlace;
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
        g_appConfigSocMic1 = 0;
        g_appConfigSocMic2 = NO_MIC;
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
    case STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB:  ///App设置语言唤醒是否使能
        gUserParam.apolloEnable = ind->payload[0];
        ParamSaveUserPrm(&gUserParam);
        break;
    case STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB:
        gUserParam.sensorEnable = ind->payload[0];
#ifdef HAVE_EM20168
        EM20168Power(gUserParam.sensorEnable);   ///App设置是否佩戴使能
#endif
        ParamSaveUserPrm(&gUserParam);
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
        if(1 == gUserParam.apolloEnable)
            apolloWakeupPower(1);
        break;
    case APP_GAIA_DISCONNECTED:
        DEBUG_LOG("GAIA disconnect from phone");
        progRun->gaiaStat  = 0;
        apolloWakeupPower(0);
        break;

    case APP_BTN_DOUBLE_TAP:
        subUiKeyDoubleTap(progRun);
        break;

    case INIT_CFM:
        DEBUG_LOG("appSubUiHandleMessage INIT_CFM start");    /* Get microphone sources */
        appUiPowerSave((TRUE==max20340_GetConnect()) ? POWER_MODE_IN_CASE : POWER_MODE_OUT_CASE);
        register_apollo_wakeup_cb(apolloWakeupCallback);                       //注册apollo唤醒函数
        appGaiaClientRegister(appGetUiTask());                         // 获取GAIA的连接断开消息
        /// todo hjs 暂时不启用自动配对
#ifndef TWS_DEBUG
        MessageSendLater(task, APP_INTERNAL_HANDSET_PAIR, NULL, 500);  // 启动广播(没有手机连接时)
#endif
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
        // 盒盖打开，系统不要进入低功耗
        appUiPowerSave((1==progRun->caseLidOpen) ? POWER_MODE_IN_CASE_OPEN : POWER_MODE_IN_CASE);
        subUiCasestat2Gaia(id, progRun);
        break;
#ifdef TWS_DEBUG
    case APP_PAIR_HEADSET:
        DEBUG_LOG("plc call pair headset");
        appSmPairHandset();
        break;
#endif
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

    case APP_INTERNAL_POWERSAVECHG:
        appUiPowerSaveSync();
        break;

    case STAROT_RECORD_CALLIN_STOP_STATUS_REPORT:
    case STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT:
    case STAROT_RECORD_STOP_STATUS_REPORT:
        subUiStopReport2Gaia(id);
        break;
    case APP_ASSISTANT_AWAKEN:
        subUiStartAssistant2Gaia(id, progRun);
        break;
    case APP_ATTACH_PLC_IN: {
        DEBUG_LOG("parse APP_ATTACH_PLC_IN event");
#ifdef TWS_DEBUG
        phyStateTaskData* phy_state = appGetPhyState();
        MessageSend(&phy_state->task, CHARGER_MESSAGE_ATTACHED, NULL);
        gProgRunInfo.realInCase = TRUE;
#endif
        appUiPowerSave(POWER_MODE_IN_CASE);
    }
        break;

    case APP_ATTACH_PLC_OUT:  {
        DEBUG_LOG("parse APP_ATTACH_PLC_OUT event");
#ifdef TWS_DEBUG
        phyStateTaskData* phy_state = appGetPhyState();
        MessageSend(&phy_state->task, CHARGER_MESSAGE_DETACHED, NULL);
        gProgRunInfo.realInCase = FALSE;
#endif
        appUiPowerSave(POWER_MODE_OUT_CASE);
    }

        // 入耳 出耳
    case APP_PSENSOR_INEAR:
        appUiPowerSave(POWER_MODE_IN_EAR);
        break;
    case APP_PSENSOR_OUTEAR:
        appUiPowerSave(POWER_MODE_OUT_CASE);
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
#ifdef TWS_DEBUG
    progRun->realInCase = TRUE;
#endif
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
        if(pCall->number && pCall->size_number < size_number)
            pfree(pCall->number), pCall->number = NULL;

        if(pCall->number == NULL)
            pCall->number = PanicUnlessMalloc(size_number+1);

        pCall->size_number = size_number;
        pCall->income      = income;
        memcpy(pCall->number, number, size_number);
        pCall->number[size_number] = 0;
        DEBUG_LOG("Call[%d]Number[%d]: %c %c %c %c", progRun->callIndex, size_number,
            number[7], number[8], number[9], number[10]);
    }

    if(name && size_name > 0) {
        if(pCall->name && pCall->size_name < size_name)
            pfree(pCall->name), pCall->name = NULL;

        if(pCall->name == NULL)
            pCall->name = PanicUnlessMalloc(size_name+1);

        pCall->size_name = size_name;
        memcpy(pCall->name, name, size_name);
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

    if(1 == progRun->recStat)
        MessageSend(&appGetUi()->task, STAROT_RECORD_CALLIN_STOP_STATUS_REPORT, 0);

}

/* Show HFP outcoming call */
void appUiHfpCallOutcomingActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->callIndex = MAX_CALLIN_INFO;  // 设置为无效值

    progRun->dial_stat  &= ~DIAL_ST_INACT;
    progRun->dial_stat  |= DIAL_ST_OUT;

    MessageSend(&appGetUi()->task, APP_CALLOUT_ACT, 0);

    if(1 == progRun->recStat)
        MessageSend(&appGetUi()->task, STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT, 0);

}

/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_IN | DIAL_ST_ACT);

    if (isEnd) {
        MessageSend(&appGetUi()->task, APP_CALLIN_INACT, 0);
    }
}

/* Cancel HFP outcoming call */
void appUiHfpCallOutcomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_OUT | DIAL_ST_ACT);

    if (isEnd) {
        MessageSend(&appGetUi()->task, APP_CALLOUT_INACT, 0);
    }
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
void appUiCaseStatus(int16 lidOpen, int16 keyDown, int16 keyLong, int16 iElectrity, uint16 bitEars)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    DEBUG_LOG("CASE:%d key=%d %d %d", lidOpen, keyDown, keyLong, iElectrity);

    if(appConfigIsLeft()) {  // 只考虑对耳机是否在
        if((bitEars & 0x20))  // mask BIT
            progRun->peerPlace = ((bitEars & 0x02)) ? 1 : 0;
    } else {
        if((bitEars & 0x10))  // Mask BIT
            progRun->peerPlace = ((bitEars & 0x01)) ? 1 : 0;
    }

    if(lidOpen >= 0)
        progRun->caseLidOpen = (1 == lidOpen) ? 1 : 0;

    if(keyDown >= 0) {
        progRun->caseKeyDown = (1 == keyDown) ? 1 : 0;
        if (appConfigIsLeft()) {
            /// 如果当前是左边耳机，发送配对信息
            DEBUG_LOG("call left pair headset");
            MessageSend(&appGetUi()->task, APP_PAIR_HEADSET, 0);
        } else {
            /// 如果当前是右边耳机，查看左耳机是否在，如果不在，执行配对
            if (progRun->peerPlace == 0) {
                /// send to
                DEBUG_LOG("call right pair headset");
                MessageSend(&appGetUi()->task, APP_PAIR_HEADSET, 0);
            }
        }
    }

    if(keyLong >= 0) {
        progRun->caseKeyLong = (1 == keyLong) ? 1 : 0;
        /// 清理命令开始
    }

    if(iElectrity >= 0)
        appUiBatteryStat(-1, -1, iElectrity);

//    MessageSend(&appGetUi()->task, APP_CASE_REPORT_INFO, 0);
}

void appUiCaseVersion(uint16 hwVer, uint16 swVer)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->caseHWver = hwVer;
    progRun->caseSWver = swVer;

    MessageSend(&appGetUi()->task, APP_CASE_REPORT_VERSION, 0);
}

// 盒子发送过来对方耳机地址
void appUiCaseSetPeerBtAddr(uint8 *addrbuf)
{
    typed_bdaddr taddr;

    if(!addrbuf) return;

    DEBUG_LOG("PeerAddr:%02x:%02x:%02x:%02x:%02x:%02x",
        addrbuf[0],addrbuf[1],addrbuf[2],addrbuf[3],addrbuf[4],addrbuf[5]);
    taddr.addr.nap = addrbuf[0] | (addrbuf[1] << 8);
    taddr.addr.uap = addrbuf[2];
    taddr.addr.lap = (addrbuf[3] << 16) | (addrbuf[4] << 8) || addrbuf[5];

    MessageSend(&appGetUi()->task, APP_CASE_SET_BTINFO, 0);

    ParamSavePeerAddr(&taddr);
}


void appUiDeepSleepMode(bool enable)    // 允许进入SLEEP模式
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(FALSE == enable) {
        if(1 != progRun->disableSleep) {
            DEBUG_LOG("DeepSeep Disable");
            VmDeepSleepEnable(FALSE);
            progRun->disableSleep = 1;
        }
    }
    else {
        if(0 != progRun->disableSleep) {
            DEBUG_LOG("DeepSeep Enable");
            VmDeepSleepEnable(TRUE);
            progRun->disableSleep = 0;
        }
    }
}

void appUiPowerSave(PowerSaveMode mode)           // 省电模式
{
    uint16 timeout;
    ProgRIPtr  progRun = appSubGetProgRun();

    if(POWER_MODE_IN_CASE == mode || POWER_MODE_IN_CASE_OPEN == mode ) {
        appUiDeepSleepMode(FALSE);  // 进入盒子，立即禁进入低功耗
        timeout = 2000;
    }
    else if(POWER_MODE_OUT_CASE == mode)
        timeout = 1000;
    else if(POWER_MODE_IN_EAR == mode)
        timeout = 500;
    else
        timeout = 10;

    //这个涵数就是为了延时，无论模式怎么变化
    progRun->iPowerSaveMode   = mode;
    MessageCancelAll(appGetUiTask(), APP_INTERNAL_POWERSAVECHG);
    MessageSendLater(appGetUiTask(), APP_INTERNAL_POWERSAVECHG, 0, timeout);

}

void appUiPowerSaveSync(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    DEBUG_LOG("PowerSaveSync mode=%d", progRun->iPowerSaveMode);

    switch(progRun->iPowerSaveMode) {
    case POWER_MODE_IN_CASE_OPEN:
    case POWER_MODE_IN_CASE:
        EM20168Power(0);             // 接近光
        Lis2dw12Power(0);            // TAP
        lis25Power(0);               // 骨麦
        apolloWakeupPower(0);        // APO2

        appUiDeepSleepMode(FALSE);   // PLC通信不能进入低功耗
        break;
    case POWER_MODE_OUT_CASE:
        EM20168Power(1);             // 接近光
#ifdef TWS_DEBUG
        Lis2dw12Power(0);            // TAP
#else
        Lis2dw12Power(1);            // TAP
#endif
        lis25Power(1);               // 骨麦
        apolloWakeupPower(1);        // APO2

        appUiDeepSleepMode(TRUE);    // PLC
        break;
    case POWER_MODE_IN_EAR:
        EM20168Power(1);             // 接近光
        Lis2dw12Power(1);            // TAP
        lis25Power(1);               // 骨麦
        apolloWakeupPower(1);        // APO2

        appUiDeepSleepMode(TRUE);    // PLC
        break;
    }
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

uint8 appUiGetPower(void)      // 获取当前耳机电量
{
    ProgRIPtr  progRun = appSubGetProgRun();

//    if(CHARGE_ST_FIN == progRun->chargeStat)   // XXX 是否需要考虑当前充电状态 ？
    return progRun->iElectrity;
}

//static bool deviceRealInCase = TRUE;

bool  appUIDeviceRealInCase(void) {
#ifdef TWS_DEBUG
    return gProgRunInfo.realInCase;
#else
    return TRUE;
#endif
}

void appUIBudsPosition(int type) {
    DEBUG_LOG("call appUIBudsPosition type is :%d", type);
    phyStateTaskData* phy_state = appGetPhyState();
    if (1 == type) {
        MessageSend(&phy_state->task, CHARGER_MESSAGE_ATTACHED, NULL);
#ifdef TWS_DEBUG
        gProgRunInfo.realInCase = TRUE;
#endif
    } else if (2 == type) {
        MessageSend(&phy_state->task, CHARGER_MESSAGE_DETACHED, NULL);
#ifdef TWS_DEBUG
        gProgRunInfo.realInCase = FALSE;
#endif
    }
}

void appUICaseEvent(int type)
{
    DEBUG_LOG("call appUICaseEvent type is :%d", type);
    if (1 == type) {
    } else if (2 == type) {
    } else if (3 == type) {
    }
}

uint8 appUIGetPowerCaseState(void)
{
    return gProgRunInfo.powerCaseState;
}

/*! At the end of every tone, add a short rest to make sure tone mxing in the DSP doens't truncate the tone */
#define RINGTONE_STOP  RINGTONE_NOTE(REST, HEMIDEMISEMIQUAVER), RINGTONE_END
const ringtone_note app_tone_wakeup[];
const ringtone_note app_tone_wakeup[] =
{
    RINGTONE_TIMBRE(sine), RINGTONE_DECAY(16),
    RINGTONE_NOTE(A7, SEMIQUAVER),
    RINGTONE_NOTE(B7, SEMIQUAVER),
    RINGTONE_NOTE(C7, SEMIQUAVER),
    RINGTONE_STOP
};

int apolloWakeupCallback(void)
{
    if(1 == g_commuType){
        char* buff = "check WAKEUP SUSS";
        CommpcMessage((uint8*)buff, 18);
    }

    ProgRIPtr  progRun = appSubGetProgRun();
    if((0 == progRun->recStat) &&
            ((progRun->dial_stat & (DIAL_ST_IN|DIAL_ST_OUT|DIAL_ST_ACT)) == 0)){
        MessageSend(&appGetUi()->task, APP_ASSISTANT_AWAKEN, 0);
    }
    appUiPlayToneCore(app_tone_wakeup, FALSE, TRUE, NULL, 0);
    return 0;
}

bool appKymeraApolloIsRun(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    return (progRun->apolloWakeup != 0) ? TRUE : FALSE;
}

void apolloWakeupPower(int enable)        // 开启或停止 APO2
{
    ProgRIPtr  progRun = appSubGetProgRun();

    if(enable) {
        if(1 == gUserParam.apolloEnable && 1 == progRun->gaiaStat){          // 系统配置了启动
            progRun->apolloWakeup = 1;
            appSubUISetMicbias(TRUE);
            OperatorFrameworkEnable(MAIN_PROCESSOR_ON);    // 需要放在SetMicbias之后，否则容易出现2.1V
            apollo_evoke();
        }
    }
    else {
        if(1 == progRun->apolloWakeup) {          // 当前已经启动
            apollo_sleep();                  // APO2,可能不成功
            appSubUISetMicbias(FALSE);
            OperatorFrameworkEnable(MAIN_PROCESSOR_OFF);
            progRun->apolloWakeup = 0;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////
///     PYDBG 调试使用
/////////////////////////////////////////////////////////////////////////////////////////
void do_bias(int value);
void do_bias(int value)
{
    if(value) {
        appSubUISetMicbias(TRUE);
        OperatorFrameworkEnable(MAIN_PROCESSOR_ON);    // 需要放在SetMicbias之后，否则容易出现2.1V
    }
    else {
        appSubUISetMicbias(FALSE);
        OperatorFrameworkEnable(MAIN_PROCESSOR_OFF);
    }
}

#endif
