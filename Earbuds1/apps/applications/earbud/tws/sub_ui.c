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
#include "peer.h"
#include "param.h"

extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);
extern bool appKymeraRecordIsRun(void);
extern void disable_audio_forward(bool disable);
#ifdef HAVE_MAX20340
extern bool max20340_GetConnect(void);
extern void max20340_notify_current_status(void);
#endif
void HfpDialNumberRequest(hfp_link_priority priority, uint16 length, const uint8 *number);
void appUiBatteryStat(uint8 lbatt, uint8 rbatt, uint16 cbatt);
void appSubUISetMicbias(int set);

static void appUIGetPowerInfo(ProgRIPtr  progRun, uint8* arr);
static uint8 appUIGetPositionInfo(void);
static uint8 appUIGetMasterSlave(void);
static uint8 appUIGetPeerPosition(void);
static uint8 appUIGetCurrentPosition(void);

static uint8 appUIGetConnectStatusInfo(void);
static uint8 appUIGetCurrentConnectStatusInfo(void);
static uint8 appUIGetPeerConnectStatusInfo(void);
static uint8 appUIGetCaseConnectStatusInfo(void);
static void appNotifyPeerDeviceConfig(uint16 source);
extern bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id, uint16 status,
                              uint16 payload_length, uint8 *payload);
static void subUiEarInOutHandle(ProgRIPtr progRun, bool isIn);

/*! At the end of every tone, add a short rest to make sure tone mxing in the DSP doens't truncate the tone */
#define RINGTONE_STOP  RINGTONE_NOTE(REST, HEMIDEMISEMIQUAVER), RINGTONE_END

static const ringtone_note app_tone_music[] =
{
    RINGTONE_TIMBRE(sine), RINGTONE_DECAY(16),
    RINGTONE_NOTE(A7, SEMIQUAVER),
    RINGTONE_STOP
};
static const ringtone_note app_tone_wakeup[] =
{
    RINGTONE_TIMBRE(sine), RINGTONE_DECAY(16),
    RINGTONE_NOTE(A7, SEMIQUAVER),
    RINGTONE_NOTE(B7, SEMIQUAVER),
    RINGTONE_NOTE(C7, SEMIQUAVER),
    RINGTONE_STOP
};

#define USE_TWO_MIC

ProgRunInfo gProgRunInfo;

#ifdef USE_TWO_MIC
uint8 g_appConfigSocMic1 = 0, g_appConfigSocMic2 = 1;      // 设置为 NO_MIC，就是不使用这个MIC（使用单MIC）
#else
uint8 g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;      // 设置为 NO_MIC，就是不使用这个MIC（使用单MIC）
#endif

/* BLE 已经连接到手机，则不需要修改广播内容, 没有连接到手机则信息不需要发送出去 */
#define BLE_CONNECTED_PHONE()  (NULL != appGetGaiaTransport())
/* 如果APP没有初始化结束，很多消息不能处理 */
#define RETURN_APP_NOT_INIT()  do{if(appInitCompleted() == FALSE) return; }while(0)
extern UserPrmPtr GetUserParam(void);

#define DBCLINK_LOG          DEBUG_LOG

static void subUiReConnectHandset(ProgRIPtr progRun)
{
    if(progRun)
        return;


}

// isTap: TRUE: 按键唤醒  FALSE:语音唤醒
static void subUiVoiceTapWakeup(ProgRIPtr progRun, bool isTap)
{
    UserPrmPtr prm = GetUserParam();
    uint8 keyFunc = (TRUE == appConfigIsLeft()) ? prm->lKeyFunc : prm->rKeyFunc;

    if(appDeviceIsHandsetConnected()) {                  // 主耳机
        DBCLINK_LOG("wakeappM,isTap=%d gaia=%d peerGaia=%d", isTap, progRun->gaiaStat, progRun->peerGaiaStat);

        if (FALSE == isTap) {                           // 主耳机，APO唤醒
            if(prm->assistantType == ASSISTANT_TYPE_APP)  {
                if(progRun->gaiaStat == 1)              // 已经连接APP
                    MessageSend(appGetUiTask(), APP_ASSISTANT_AWAKEN, 0);
                else {
                    appUiPlayPrompt(PROMPT_CONNECT_APP);        // 请连接手机APP;
                    return;
                }
            }
            else {
                MessageSend(appGetUiTask(), APP_ASSISTANT_SYSTEM, 0);
            }
        }
        else {                                     // 主耳机，TAP唤醒
            if(TAP_WACKUP == keyFunc) {
                if(progRun->gaiaStat == 1)
                    MessageSend(appGetUiTask(), APP_ASSISTANT_TAP_AWAKEN, 0);
                else {
                    appUiPlayPrompt(PROMPT_CONNECT_APP);        // 请连接手机APP;
                    return;
                }
            }
            else if(TAP_SYSTEM == keyFunc)
                MessageSend(appGetUiTask(), APP_ASSISTANT_TAP_SYSTEM, 0);
            else {
                DBCLINK_LOG("tap no set wakeup");
                return;
            }
        }
    }
    else {                                              // 副耳机
        DBCLINK_LOG("wakeapp,isTap=%d gaia=%d peerGaia=%d", isTap, progRun->gaiaStat, progRun->peerGaiaStat);

        if(!appDeviceIsPeerAvrcpConnectedForAv() || !appPeerSyncIsComplete() || !appPeerSyncIsPeerHandsetAvrcpConnected()) {
            appUiPlayPrompt(PROMPT_CONNECT_APP);        // 没有与对方连接或对方没有连接手机,提示请连接手机APP;
            return;
        }

        if (FALSE == isTap) {                           // 副耳机，APO唤醒
            if(prm->assistantType == ASSISTANT_TYPE_APP) {
                if(1 == progRun->peerGaiaStat)  {       // 对方已经启动APP
                    appPeerSigTxWakeupApp(1);  // 1为APO
                }
                else {
                    appUiPlayPrompt(PROMPT_CONNECT_APP);        // 没有与对方连接或对方没有连接手机,提示请连接手机APP;
                    return;
                }
            }
            else {
                appPeerSigTxWakeupSys(1);      // 1为APO
            }
        }
        else {                                          // 副耳机，TAP唤醒
            if(TAP_WACKUP == keyFunc) {
                if(1 == progRun->peerGaiaStat)  {       // 对方已经启动APP
                    appPeerSigTxWakeupApp(0);  // 0为tap
                }
                else {
                    appUiPlayPrompt(PROMPT_CONNECT_APP);        // 没有与对方连接或对方没有连接手机,提示请连接手机APP;
                    return;
                }
            }
            else if(TAP_SYSTEM == keyFunc)
                appPeerSigTxWakeupSys(0);  // 0为tap
            else {
                DBCLINK_LOG("tap no set wakeup");
                return;
            }
        }
    }
}


static void subUiDoubleClickAB(ProgRIPtr progRun, bool isLeft)
{
    bdaddr peer_addr;
    UserPrmPtr prm = GetUserParam();
    uint8 handled, keyFunc = (TRUE == isLeft) ? prm->lKeyFunc : prm->rKeyFunc;

    // 单耳机使用时


    // 主副耳机是否已经正常配对成功
    appDeviceGetPeerBdAddr(&peer_addr);


    // 两只耳机都没有与手机连接,提示请连接手机
    if((FALSE == appPeerSyncIsPeerHandsetHfpConnected()) && (FALSE == appDeviceIsHandsetConnected())) {
        subUiReConnectHandset(progRun);

#ifndef TWS_DEBUG
        if(!appSmIsPairing())
            appSmPairHandset();
#endif

        appSmConnectHandset();
        appUiAvConnect();

        DBCLINK_LOG("DBClink Two earbuds not connect phone");
        goto key_done;
    }

    handled = 1;  /* 设置为已经处理 */
    /* 电话操作时的控制，不能自己定义 */
    if (appHfpIsCallActive())   /* If voice call active, hangup */
        appHfpCallHangup();
    else if (appScoFwdIsReceiving() && !appScoFwdIsCallIncoming()) /* Sco Forward can be streaming a ring tone */
        appScoFwdCallHangup();
    else if (appHfpIsCallOutgoing()) /* If outgoing voice call, hangup */
        appHfpCallHangup();
    else if (appHfpIsCallIncoming())    /* If incoming voice call, accept */
        appHfpCallAccept();
    else if (appScoFwdIsCallIncoming())
        appScoFwdCallAccept();
    else {     /* 非通话状态时的按键处理 */
        handled = 0;   /* 设置为未处理 */
    }

    /* 已经处理或双击关闭 */
    if(handled || TAP_DISABLED == keyFunc) {
        DBCLINK_LOG("DBClink PhoneCall(%d) or keyDisable(%d)", handled, keyFunc);
        goto key_done;
    }

    /* 音乐播放中 */
    if(appDeviceIsHandsetAvrcpConnected() ||
        (appDeviceIsPeerAvrcpConnectedForAv() && appPeerSyncIsComplete() && appPeerSyncIsPeerHandsetAvrcpConnected()))
    {
        if( keyFunc == TAP_PREVIOUS_TRACK){          /*音乐播放中 -->音乐前一首 */
            appUiPlayToneCore(app_tone_music, FALSE, TRUE, NULL, 0);
            appAvBackward();
            goto key_done;
        }
        else if(keyFunc == TAP_NEXT_TRACK){          /*音乐播放中 -->音乐后一首 */
            appUiPlayToneCore(app_tone_music, FALSE, TRUE, NULL, 0);
            appAvForward();
            goto key_done;
        }
        else if(keyFunc == TAP_PLAY_PAUSE){          /*音乐播放中 -->暂停或播放 */
            appAvPlayToggle(TRUE);
            goto key_done;
        }
    }

    subUiVoiceTapWakeup(progRun, TRUE);

key_done:
    return;
}

static void subUiKeyDoubleTap(ProgRIPtr progRun)
{
    DEBUG_LOG("APP_MFB_BUTTON_PRESS outcase=0x%x", progRun, appSmIsOutOfCase());
    if (appSmIsOutOfCase())
    {
        subUiDoubleClickAB(progRun, appConfigIsLeft() ? TRUE : FALSE);
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

static void subUiStarttAssistantSystem(bool isTap)
{
    (void)isTap;
    HfpVoiceRecognitionEnableRequest(hfp_primary_link, appGetHfp()->voice_recognition_request = TRUE);
}

static int16 subUiStartAssistant2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    /* 唤醒系统助手，需要告诉APP是 APO还是TAP */
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 4);
    if(id == APP_ASSISTANT_AWAKEN){
        message->command = GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START;
        message->payload[0] = 0x02;
    }
    if(id == APP_ASSISTANT_TAP_AWAKEN){
        message->command = GAIA_COMMAND_STAROT_AI_DEVICE_REQUEST_START;
        message->payload[0] = 0x01;
    }
    MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, message);
    return 0;
}

static int16 subUiStat2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    (void)id;

    uint16 battery_level, peer_battery_level;
    appPeerSyncGetPeerBatteryLevel(&battery_level, &peer_battery_level);
    progRun->peerElectrity = appBatteryConvertLevelToPercentage(peer_battery_level);

//    phyState state = appPhyStateGetState();
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 5);

    message->command = STAROT_NOTIFY_STATUS;
    appUIGetPowerInfo(progRun, message->payload);
    message->payload[3] = appUIGetPositionInfo();//位置信息
    message->payload[4] = appUIGetConnectStatusInfo();//连接信息
//    if(appConfigIsLeft()){
//        if((uint8)progRun->gaiaStat)
//            message->payload[4] |= 0X80;
//        if(appDeviceIsHandsetAnyProfileConnected())
//            message->payload[4] |= 0X40;
//        if(appDeviceIsPeerConnected()) {
//            message->payload[4] |= 0X20;
//            message->payload[4] |= 0X04;
//        }
//        if(appPeerSyncIsPeerHandsetA2dpConnected() || appPeerSyncIsPeerHandsetAvrcpConnected()
//                || appPeerSyncIsPeerHandsetHfpConnected())
//            message->payload[4] |= 0X08;
//    }
//    else {
//        if((uint8)progRun->gaiaStat)
//            message->payload[4] |= 0X10;
//        if(appDeviceIsHandsetAnyProfileConnected())
//            message->payload[4] |= 0X08;
//        if(appDeviceIsPeerConnected()) {
//            message->payload[4] |= 0X20;
//            message->payload[4] |= 0X04;
//        }
//        if(appPeerSyncIsPeerHandsetA2dpConnected() || appPeerSyncIsPeerHandsetAvrcpConnected()
//                || appPeerSyncIsPeerHandsetHfpConnected())
//            message->payload[4] |= 0X40;
//    }
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
        if (appHfpIsCallActive()) {
            appHfpCallHangup();
        } else {
            appHfpCallReject();
        }
        break;

    case STAROT_AI_USER_START_RECORD:               ///设备开始录音
        progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
        ///AI录音时，使用的是单MIC
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;
        appKymeraRecordStart();
#endif
        break;

    case STAROT_AI_USER_STOP_RECORD:               ///设备停止录音
        progRun->recStat  = 0;
#ifdef CONFIG_REC_ASSISTANT
        appKymeraRecordStop();
#ifdef USE_TWO_MIC
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = 1;
#else
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;
#endif
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
    case STAROT_BASE_INFO_SET_APOLLO_WAKEUP_ENB: {  ///App设置语言唤醒是否使能
        APP_STAROT_WAKEUP_CONFIG_IND_T* m = (APP_STAROT_WAKEUP_CONFIG_IND_T*)message;
        gUserParam.apolloEnable = m->apollo_enable;
        gUserParam.assistantType = m->assistant_type;
        gUserParam.assistantModifyTime = m->timestamp;
        ParamSaveUserPrm(&gUserParam);
        if (0 < gUserParam.apolloEnable) { /// 使能
            apolloWakeupPower(1);
        } else { /// 禁用
            apolloWakeupPower(0);
        }

        appNotifyPeerDeviceConfig(m->messageFrom);
    }
        break;
    case STAROT_BASE_INFO_SET_ADORN_CHEAK_ENB: {
        APP_STAROT_WEAR_CONFIG_IND* m = (APP_STAROT_WEAR_CONFIG_IND*)message;
        gUserParam.sensorEnable = m->wear_enable;
        gUserParam.sensorModifyTime = m->timestamp;
#ifdef HAVE_EM20168
        EM20168Power(gUserParam.sensorEnable);   ///App设置是否佩戴使能
#endif
        ParamSaveUserPrm(&gUserParam);
        appNotifyPeerDeviceConfig(m->messageFrom);
    }
        break;
    case STAROT_APP_CONTROL_PREVIOUS_TRACK:      ///App控制上一首
        appUiPlayToneCore(app_tone_music, FALSE, TRUE, NULL, 0);
        appAvBackward();
        break;
    case STAROT_APP_CONTROL_NEXT_TRACK:          ///App控制下一首
        appUiPlayToneCore(app_tone_music, FALSE, TRUE, NULL, 0);
        appAvForward();
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
        /// 需要根据实际使用的容量去计算
//        DEBUG_LOG("power precent is :%02X", ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent);
        if(progRun->iElectrity == ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent)
            break;
        progRun->iElectrity = ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent;
        if (appPeerSyncIsComplete())
            appPeerSyncSend(FALSE);
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
        appPeerSigTxDataCommandUi(PEERTX_CMD_SYNCGAIA, 1);
        break;
    case APP_GAIA_DISCONNECTED:
        DEBUG_LOG("GAIA disconnect from phone");
        progRun->gaiaStat  = 0;
#ifdef CONFIG_REC_ASSISTANT
        if (appKymeraRecordIsRun() == TRUE){
            appKymeraRecordStop();
#ifdef USE_TWO_MIC
            g_appConfigSocMic1 = 0, g_appConfigSocMic2 = 1;
#else
            g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;
#endif
        }
        ///todo 添加通话录音停止操作
#endif
        apolloWakeupPower(0);
        appPeerSigTxDataCommandUi(PEERTX_CMD_SYNCGAIA, 0);
        break;

    case APP_BTN_DOUBLE_TAP:
        subUiKeyDoubleTap(progRun);
        break;

    case INIT_CFM:
        DEBUG_LOG("appSubUiHandleMessage INIT_CFM start");    /* Get microphone sources */
#ifdef HAVE_MAX20340
        appUiPowerSave((TRUE==max20340_GetConnect()) ? POWER_MODE_IN_CASE : POWER_MODE_OUT_CASE);
        max20340_notify_current_status();
#endif
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_UPGRADE);
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
    case APP_PAIR_HEADSET:
        DEBUG_LOG("plc call pair headset");
        progRun->handsetPair = 0;           // 准备配对，设置为unknown
#ifdef TWS_DEBUG
        appSmPairHandset();
#endif
        break;
   case APP_RESET_FACTORY:
       DEBUG_LOG("plc call reset headset");
#ifdef TWS_DEBUG
       appSmFactoryReset();
#endif
       break;
    case APP_CASE_OPEN:
        DEBUG_LOG("plc call case open");
#ifdef TWS_DEBUG
        appConnRulesResetEvent(RULE_EVENT_CASE_OPEN);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_CASE_OPEN);
#endif
        break;
    case APP_CASE_CLOSE:
        DEBUG_LOG("plc call case close");
#ifdef TWS_DEBUG
        if (appGaiaIsConnect()) {
            DEBUG_LOG("call appGaiaDisconnect and send GAIA_COMMAND_STAROT_BASE_INFO_ACTIVE_DISCONNECT");
            appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_BASE_INFO_ACTIVE_DISCONNECT, 0xfe, 0, NULL);
        }
        ///  直接发送之后，又立即断开，导致数据没有发送出去，现在需要使用延迟发送
        /// todo hjs 看看如果没有延迟，消息是否能发送出去
        /// 设置规则去处理
        MessageCancelAll(&appGetUi()->task, APP_CASE_CLOSE_LATER);
        MessageSendLater(&appGetUi()->task, APP_CASE_CLOSE_LATER, NULL, 50);
#endif
        break;
    case APP_CASE_CLOSE_LATER:
        appConnRulesResetEvent(RULE_EVENT_CASE_CLOSE);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_CASE_CLOSE);
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

    case APP_INTERNAL_POWERSAVECHG:
        appUiPowerSaveSync();
        break;

    case STAROT_RECORD_CALLIN_STOP_STATUS_REPORT:
    case STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT:
    case STAROT_RECORD_STOP_STATUS_REPORT:
        subUiStopReport2Gaia(id);
        break;

     ///////////////////////////////////////////////////////////////////////
     ///  此处理几个唤醒ID，仅执行功能，左右耳机或配置 都会执行这几个调用
    case APP_ASSISTANT_AWAKEN:         //APO 语音唤醒助手APP
        subUiStartAssistant2Gaia(id, progRun);
        break;
    case APP_ASSISTANT_TAP_AWAKEN:     //敲击 语音唤醒助手APP
        subUiStartAssistant2Gaia(id, progRun);
        break;
    case APP_ASSISTANT_SYSTEM:         //APO 敲击唤醒系统助手
        subUiStarttAssistantSystem(FALSE);
        break;
    case APP_ASSISTANT_TAP_SYSTEM:     //敲击唤醒系统助手
        subUiStarttAssistantSystem(TRUE);
        break;

    case APP_ATTACH_PLC_IN: {
        DEBUG_LOG("parse APP_ATTACH_PLC_IN event");
#ifdef TWS_DEBUG
       // if (appSmIsOutOfEar()) {
//        phyStateTaskData* phy_state = appGetPhyState();
//        MessageCancelAll(&phy_state->task, CHARGER_MESSAGE_ATTACHED);
//        MessageSendLater(&phy_state->task, CHARGER_MESSAGE_ATTACHED, NULL, 50);
        if (appGaiaIsConnect()) {
            DEBUG_LOG("call appGaiaDisconnect and send GAIA_COMMAND_STAROT_BASE_INFO_ACTIVE_DISCONNECT");
            appGaiaSendPacket(GAIA_VENDOR_STAROT, GAIA_COMMAND_STAROT_BASE_INFO_ACTIVE_DISCONNECT, 0xfe, 0, NULL);
        }
//        gProgRunInfo.realInCase = TRUE;
       // }
#endif
//        appUiPowerSave(POWER_MODE_IN_CASE);
    }
        break;

    case APP_ATTACH_PLC_OUT:  {
        DEBUG_LOG("parse APP_ATTACH_PLC_OUT event");
#ifdef TWS_DEBUG
       // if (appSmIsInCase()) {
//        phyStateTaskData* phy_state = appGetPhyState();
//        MessageCancelAll(&phy_state->task, CHARGER_MESSAGE_DETACHED);
//        MessageSend(&phy_state->task, CHARGER_MESSAGE_DETACHED, NULL);
//        gProgRunInfo.realInCase = FALSE;
       // }
#endif
        /// 从充电盒中取出，默认充电盒之后是关闭的，放入的时候，会收到case状态，打开的，会使这个事件失效
//        progRun->caseLidOpen = 0;
//#ifdef DEVELOPE_BOARD
//        appUiPowerSave(POWER_MODE_IN_EAR);
//#else
//        appUiPowerSave(POWER_MODE_OUT_CASE);
//#endif
    }
        break;

        // 入耳 出耳
    case APP_PSENSOR_INEAR:
//        appUiPowerSave(POWER_MODE_IN_EAR);
        subUiEarInOutHandle(progRun, TRUE);
        break;
    case APP_PSENSOR_OUTEAR:
        subUiEarInOutHandle(progRun, FALSE);
//        appUiPowerSave(POWER_MODE_OUT_CASE);
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

    // 运行到这个地方时外设都为正常打开状态
    apollo_sleep();

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

//如果此时有录音
#ifdef CONFIG_REC_ASSISTANT
    if (appKymeraRecordIsRun()){
        appKymeraRecordStop();
#ifdef USE_TWO_MIC
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = 1;
#else
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;
#endif
    }
#endif

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

//如果此时有录音
#ifdef CONFIG_REC_ASSISTANT
    if (appKymeraRecordIsRun()){
        appKymeraRecordStop();
#ifdef USE_TWO_MIC
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = 1;
#else
        g_appConfigSocMic1 = 0, g_appConfigSocMic2 = NO_MIC;
#endif
    }
#endif

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

/* EDR 配对成功与否 */
void appUiPairingComplete(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->handsetPair = 1;
    appUiPlayPrompt(PROMPT_PAIRING_SUCCESSFUL);
}


/* Play pairing failed prompt */
void appUiPairingFailed(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->handsetPair = 2;
    appUiPlayPrompt(PROMPT_PAIRING_FAILED);
}


///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
// 数值小于0，表示不确定，不设置
void appUiCaseStatus(int16 lidOpen, int16 keyDown, int16 keyLong, int16 iElectrity, uint16 bitEars)
{
    ProgRIPtr  progRun = appSubGetProgRun();

//    DEBUG_LOG("CASE:%d key=%d %d %d", lidOpen, keyDown, keyLong, iElectrity);

    if(appConfigIsLeft()) {  // 只考虑对耳机是否在
        if((bitEars & 0x20))  // mask BIT
            progRun->peerPlace = ((bitEars & 0x02)) ? 1 : 0;
    } else {
        if((bitEars & 0x10))  // Mask BIT
            progRun->peerPlace = ((bitEars & 0x01)) ? 1 : 0;
    }

    if (0X00 == appGetState()) {
        return;
    }

    if(lidOpen >= 0) {
        static uint8 beforeStatus = 0xFF;                   // 先设置为0xFF,这样第一次过来就能发送信息了
        if (beforeStatus != lidOpen) {
            beforeStatus = lidOpen;
            progRun->caseLidOpen = (1 == lidOpen) ? 1 : 0;
            /// 之前状态和现在状态不一致，发送事件
            if (progRun->caseLidOpen > 0) {
                DEBUG_LOG("call case open");
                MessageCancelAll(&appGetUi()->task, APP_CASE_OPEN);
                MessageCancelAll(&appGetUi()->task, APP_CASE_CLOSE);
                MessageSendLater(&appGetUi()->task, APP_CASE_OPEN, NULL, 50);
            } else {
                DEBUG_LOG("call case close");
                MessageCancelAll(&appGetUi()->task, APP_CASE_OPEN);
                MessageCancelAll(&appGetUi()->task, APP_CASE_CLOSE);
                MessageSendLater(&appGetUi()->task, APP_CASE_CLOSE, NULL, 50);
            }
        }
    }

    if(keyDown >= 0) {
        progRun->caseKeyDown = (1 == keyDown) ? 1 : 0;
        if (appConfigIsLeft()) {
            /// 如果当前是左边耳机，发送配对信息
            DEBUG_LOG("call left pair headset");
            MessageSend(&appGetUi()->task, APP_PAIR_HEADSET, 0);
        } else {
            /// 如果当前是右边耳机，查看左耳机是否在，如果不在，执行配对
            if (progRun->peerPlace == 0) {
                DEBUG_LOG("call right pair headset");
                MessageSend(&appGetUi()->task, APP_PAIR_HEADSET, 0);
            }
        }
    }

    if(keyLong >= 0) {
        progRun->caseKeyLong = (1 == keyLong) ? 1 : 0;
        if (1 == progRun->caseKeyLong) {
            MessageSend(&appGetUi()->task, APP_RESET_FACTORY, 0);
        }
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
    taddr.addr.lap = (addrbuf[3] << 16) | (addrbuf[4] << 8) | addrbuf[5];

    MessageSend(&appGetUi()->task, APP_CASE_SET_BTINFO, 0);

    ParamSavePeerAddr(&taddr);
}

void appUiPowerSave(PowerSaveMode mode)           // 省电模式
{
    uint16 timeout;
    ProgRIPtr  progRun = appSubGetProgRun();

    if(POWER_MODE_IN_CASE == mode || POWER_MODE_IN_CASE_OPEN == mode ) {
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

// 入耳、出耳 事件处理 /// todo 如果佩戴检测使能，需要暂停播放音乐
void subUiEarInOutHandle(ProgRIPtr progRun, bool isIn){//
    (void)progRun;
    UNUSED(isIn);
//    TaskData* phy_task = &appGetPhyState()->task;
//    DEBUG_LOG("subUiInOutEar %d", isIn);
//    if(TRUE == isIn)   {
//        appUiPlayTone(app_tone_ear_in);
//        // MessageSend(phy_task, PROXIMITY_MESSAGE_IN_PROXIMITY, NULL);
//    } else {
//        // MessageSend(phy_task, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY, NULL);
//        appUiPlayTone(app_tone_ear_out);    //需要知道另外一只耳机是否也在耳朵中
//        if (appDeviceIsHandsetAvrcpConnected())
//            appAvPlayToggle(TRUE);
//    }
}

void appUiPowerSaveSync(void)
{
#ifdef CONFIG_BOARD_V2
    ProgRIPtr  progRun = appSubGetProgRun();

    DEBUG_LOG("PowerSaveSync mode=%d", progRun->iPowerSaveMode);

    switch(progRun->iPowerSaveMode) {
    case POWER_MODE_IN_CASE_OPEN:
    case POWER_MODE_IN_CASE:
        EM20168Power(0);             // 接近光
        Lis2dw12Power(0);            // TAP
        lis25Power(0);               // 骨麦
        apolloWakeupPower(0);        // APO2
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
        break;
    case POWER_MODE_IN_EAR:
        EM20168Power(1);             // 接近光
        Lis2dw12Power(1);            // TAP
        lis25Power(1);               // 骨麦
        apolloWakeupPower(1);        // APO2
        break;
    }
#endif
}

void appUiBatteryStat(uint8 lbatt, uint8 rbatt, uint16 cbatt)
{
    int16 iChange = 0;
    ProgRIPtr  progRun = appSubGetProgRun();

    if(lbatt >= 0 && progRun->iElectrity != lbatt ) {
        progRun->iElectrity = lbatt;
        iChange = 1;
    }

    if(rbatt >= 0 && progRun->peerElectrity != rbatt) {
        progRun->peerElectrity = rbatt;
        iChange = 1;
    }

    if(cbatt >= 0 && progRun->caseElectrity != cbatt) {
        progRun->caseElectrity = cbatt;
        iChange = 1;
    }

    if(iChange > 0)
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

int apolloWakeupCallback(void)
{
    if(1 == g_commuType){
        char* buff = "check WAKEUP SUSS";
        CommpcMessage((uint8*)buff, 18);
    }

    ProgRIPtr  progRun = appSubGetProgRun();
    progRun->iWakeupTimes  += 1;
    if((0 == progRun->recStat) &&
            ((progRun->dial_stat & (DIAL_ST_IN|DIAL_ST_OUT|DIAL_ST_ACT)) == 0)){
        if(0 == g_commuType) {
            subUiVoiceTapWakeup(progRun, FALSE);
        }
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

bool appUICaseIsOpen(void) {
    ProgRIPtr  progRun = appSubGetProgRun();
    return (progRun->caseLidOpen > 0 ? TRUE : FALSE);
}

uint8 appUIGetCurrentPosition(void) {
    phyState state = appPhyStateGetState();
    switch(state) {
        case PHY_STATE_IN_CASE:
            return 0X01 << 2;
        case PHY_STATE_OUT_OF_EAR:
        case PHY_STATE_OUT_OF_EAR_AT_REST:
            return  0X01 << 1;
        case PHY_STATE_IN_EAR:
            return  0X01 << 0;
        case PHY_STATE_UNKNOWN:
            return  0X00;
    }
    return 0X00;
}

uint8 appUIGetPeerPosition(void) {
    if (!appDeviceIsPeerConnected()) {
        return 0X00;
    }

    if(appPeerSyncIsPeerInCase() == TRUE)
        return 0X01 << 2;

    if(appPeerSyncIsPeerInCase() == FALSE && appPeerSyncIsPeerInEar() == FALSE)
        return 0X01 << 1;

    if(appPeerSyncIsPeerInEar() == TRUE)
        return  0X01 << 0;

    return 0X00;
}

uint8 appUIGetMasterSlave(void) {
    /// 暂时不实现
    return 0X00;
}

uint8 appUIGetPositionInfo(void) {
    uint8 current = appUIGetCurrentPosition();
    uint8 peer = appUIGetPeerPosition();
    uint8 ms = appUIGetMasterSlave();

    uint8 res = 0;
    if (appConfigIsLeft()) {
        res |= (current << 5) | (peer << 2);
    } else {
        res |= (peer << 5) | (current << 2);
    }

    return res | ms;
}

void appUIGetPowerInfo(ProgRIPtr  progRun, uint8 *arr) {
    // 原始的电量信息获取有问题
    // 0:left 1:right 2:case
    if (appConfigIsLeft()) {
        arr[0] = (uint8)progRun->iElectrity;
        if ((CHARGE_ST_OK==progRun->chargeStat) || (CHARGE_ST_LOW == progRun->chargeStat)) {
            arr[0] |= 0X80;
        }
        if (appDeviceIsPeerConnected()) {
            arr[1] = (uint8)progRun->peerElectrity;//对方电量
            if (appPeerSyncIsPeerInCase()) {
                if (arr[1] != 100) {
                    arr[1] |= 0X80;
                }
            }
        } else {
            arr[1] = 0XFF;
        }
    } else {
        if (appDeviceIsPeerConnected()) {
            arr[0] = (uint8)progRun->peerElectrity;//对方电量
            if (appPeerSyncIsPeerInCase()) {
                if (arr[0] != 100) {
                    arr[0] |= 0X80;
                }
            }
        } else {
            arr[0] = 0XFF;
        }
        arr[1] = (uint8)progRun->iElectrity;
        if ((CHARGE_ST_OK==progRun->chargeStat) || (CHARGE_ST_LOW == progRun->chargeStat)) {
            arr[1] |= 0X80;
        }
    }
    ///现在使用假的电量数据
//    arr[2] = (uint8)progRun->caseElectrity;//盒子电量
    arr[2] = 0X42;
}

uint8 appUIGetConnectStatusInfo(void) {
    uint8 current = appUIGetCurrentConnectStatusInfo();
    uint8 peer = appUIGetPeerConnectStatusInfo();
    uint8 ms = appUIGetCaseConnectStatusInfo();

    uint8 res = 0;
    if (appConfigIsLeft()) {
        res |= (current << 5) | (peer << 2);
    } else {
        res |= (peer << 5) | (current << 2);
    }

    return res | ms;
}

uint8 appUIGetCurrentConnectStatusInfo(void) {
    ProgRIPtr progRun = appSubGetProgRun();
    uint8 res = 0X00;

    if((uint8)progRun->gaiaStat) {
        res |= 0X01 << 2;
    }
    if(appDeviceIsHandsetAnyProfileConnected()) {
        res |= 0X01 << 1;
    }
    if(appDeviceIsPeerConnected()) {
        res |= 0X01 << 0;
    }

    return res;
}

uint8 appUIGetPeerConnectStatusInfo(void) {
    uint8 res = 0;
    if(appPeerSyncIsPeerHandsetA2dpConnected() || appPeerSyncIsPeerHandsetAvrcpConnected() || appPeerSyncIsPeerHandsetHfpConnected()) {
        res |= 0X01 << 1;
    }

    if(appDeviceIsPeerConnected()) {
        res |= 0X01 << 0;
    }

    return res;
}

uint8 appUIGetCaseConnectStatusInfo(void) {
    return 0X00;
}

//////////////////////////////////////////////////////////////////////////////////////
///     PYDBG 调试使用
/////////////////////////////////////////////////////////////////////////////////////////
void do_bias(int value);
void do_chgkey(int value);
void do_chgapo(int value);

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

void do_chgkey(int value)
{
    UserPrmPtr prm = GetUserParam();
    if(appConfigIsLeft())
        prm->lKeyFunc = value;
    else
        prm->rKeyFunc = value;

    DEBUG_LOG("keyfunc,l=%d r=%d", prm->lKeyFunc, prm->rKeyFunc);
}

void do_chgapo(int value)
{
    UserPrmPtr prm = GetUserParam();
    ProgRIPtr  progRun = appSubGetProgRun();

    prm->apolloEnable = TRUE;
    prm->assistantType = (value == 1) ? ASSISTANT_TYPE_APP : ASSISTANT_TYPE_SYSTEM;

    progRun->gaiaStat = 1;
    apolloWakeupPower(1);

    DEBUG_LOG("apollo,en=%d type=%d", prm->apolloEnable, prm->assistantType);
}

void appNotifyPeerDeviceConfig(uint16 source) {
    if (MESSAGE_FROM_APP == source) {
        /// 通知对方耳机，查找对方地址
        bdaddr peer_addr;
        if (appDeviceGetPeerBdAddr(&peer_addr)) {
            appPeerSigTxNormalConfigRequest(appGetGaiaTask(), &peer_addr);
        }
    }
}

void appSendMessageToUI(int message);

void appSendMessageToUI(int message) {
    MessageSend(&appGetUi()->task, message, 0);
}

void appGetLocalBrEdrAddress(uint8* addrbuf) {
    ProgRIPtr  progRun = appSubGetProgRun();
    addrbuf[0] = (progRun->addr.nap >> 8) & 0xFF;
    addrbuf[1] = (progRun->addr.nap & 0xFF);
    addrbuf[2] = progRun->addr.uap;
    addrbuf[3] = (progRun->addr.lap >> 16) & 0xFF;
    addrbuf[4] = (progRun->addr.lap >> 8) & 0xFF;
    addrbuf[5] = (progRun->addr.lap & 0xFF);
    for (int i = 0; i < 6; ++i)
        DEBUG_LOG("MAC ADDRESS :%d %02X", i, addrbuf[i]);
}


void appGetPeerBrEdrAddress(uint8* addrbuf) {
    bdaddr addr;
    if (appDeviceGetPeerBdAddr(&addr)) {
        addrbuf[0] = (addr.nap >> 8) & 0xFF;
        addrbuf[1] = (addr.nap & 0xFF);
        addrbuf[2] = addr.uap;
        addrbuf[3] = (addr.lap >> 16) & 0xFF;
        addrbuf[4] = (addr.lap >> 8) & 0xFF;
        addrbuf[5] = (addr.lap & 0xFF);
        for (int i = 0; i < 6; ++i)
            DEBUG_LOG("MAC ADDRESS :%d %02X", i, addrbuf[i]);
    }
}

bool appGetCaseIsOpen(void) {
    ProgRIPtr  progRun = appSubGetProgRun();
    return (progRun->caseLidOpen > 0) ? TRUE : FALSE;
}


void testPrintBrEdr(void);
void testPrintBrEdr(void) {
    uint8 addr[32];
    appGetLocalBrEdrAddress(addr);
    appGetPeerBrEdrAddress(addr);
}

void testSiri(void);
void testSiri(void) {
    HfpVoiceRecognitionEnableRequest(hfp_primary_link, appGetHfp()->voice_recognition_request = TRUE);
}

#endif
