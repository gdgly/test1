#ifdef CONFIG_STAROT
#include <panic.h>
#include <ps.h>
#include <boot.h>
#include <pmalloc.h>
#include <input_event_manager.h>

//#include "public.h"
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
#include "online_dbg.h"

extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);
extern bool appKymeraRecordIsRun(void);
extern void disable_audio_forward(bool disable);
#ifdef HAVE_MAX20340
extern bool max20340_GetConnect(void);
extern void max20340_notify_current_status(void);
#endif
void HfpDialNumberRequest(hfp_link_priority priority, uint16 length, const uint8 *number);
void appUiBatteryStat(int16 lbatt, int16 rbatt, int16 cbatt);
void appSubUISetMicbias(int set);

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
static void appUIUpgradeApplyInd(void);
static void appUICheckVersion(void);
static void appUICheckPeerVersionForUpdate(void);
static void appUIUpgradeCommit(void);
static void appUIUpgradeEnter(void);
static void appUIUpgradeExit(void);
static void appUIUpgradeNotifyCommitStatusInit(UI_APP_UPGRADE_COMMIT_STATUS* message);
static void appUIUpgradeNotifyCommitStatusDo(UI_APP_UPGRADE_COMMIT_STATUS* message);
static void appUIUpgradeNotifyCommitStatusTimeOut(UI_APP_UPGRADE_COMMIT_STATUS* message);
static void appUIUpgradeNotifyCommitStatusTimeOutGrade(void);

static int16 subUiCallIndicator2Gaia(ProgRIPtr  progRun, const CALL_INDICATOR_T* msg);
//extern bool UpgradeSMAbort(void);


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
            appUiPlayPrompt(PROMPT_PAIRING_FAILED);        // 没有与对方连接或对方没有连接手机,联接失败;
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
//        appUiAvConnect();

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

    /* 允许上一首下一首暂停手势的时候,在入耳状态下,音乐暂停,首次敲击,音乐播放 */
    if((keyFunc == TAP_PREVIOUS_TRACK)||(keyFunc == TAP_NEXT_TRACK)||(keyFunc == TAP_PLAY_PAUSE)){
        bool pause_stop = (appAvPlayStatus() == avrcp_play_status_paused) || (appAvPlayStatus() == avrcp_play_status_stopped);
        if(appDeviceIsHandsetConnected() && ((!appDeviceIsHandsetA2dpStreaming()) || pause_stop)){
            appAvPlay(FALSE);
            goto key_done;
        }
        if(!appDeviceIsHandsetConnected() && ((!appPeerSyncIsPeerHandsetA2dpStreaming()) || pause_stop)){
            appAvPlay(FALSE);
            goto key_done;
        }
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
static int16 subUiCallNumber2Gaia(ProgRIPtr progRun, const CALL_NUMBER_T* msg)
{
    UNUSED(progRun);
    MAKE_OBJECT_LEN(STAROT_DIALOG_CALL_NUMBER_T, msg->length);
    message->len = msg->length;
    memcpy(message->number, msg->number, msg->length);
    MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_NUMBER);
    DEBUG_LOG("call subUiCallNumber2Gaia STAROT_DIALOG_CALL_NUMBER send conditionally");
    MessageSendConditionally(appGetGaiaTask(), STAROT_DIALOG_CALL_NUMBER, message, subGaiaGetConnectLock());

    DEBUG_LOG("subUiCallNumber2Gaia Call Number: ");
    int i = 0;
    for (; i < msg->length; i++) printf("%c", msg->number[i]);
    printf("\n");

    return 0;
}

static int16 subUiCallIndicator2Gaia(ProgRIPtr  progRun, const CALL_INDICATOR_T* msg)
{
    /// 1.如果电话接通时，没有和APP成功建立连接，缓存电话抵达、拨号等属性消息
    /// 2.如果电话挂断，清理缓存的通话相关属性消息
    if (HFP_STATE_CONNECTED_IDLE != msg->command) {
        MAKE_OBJECT(STAROT_DIALOG_STATUS_T);
        message->status = msg->command;
        DEBUG_LOG("call subUiCallIndicator2Gaia STAROT_DIALOG_STATUS send conditionally");
        MessageId mid = (HFP_STATE_CONNECTED_ACTIVE == msg->command ? STAROT_DIALOG_STATUS_ACTIVE : STAROT_DIALOG_STATUS);
        MessageSendConditionally(appGetGaiaTask(), mid, message, subGaiaGetConnectLock());
    } else {
        disable_audio_forward(TRUE);

        MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_CALL_NUMBER);
        MessageCancelAll(appGetGaiaTask(), STAROT_DIALOG_STATUS);
        if (!subGaiaIsConnectLock()) {
            MAKE_OBJECT(STAROT_DIALOG_STATUS_T);
            message->status = msg->command;
            MessageSend(appGetGaiaTask(), STAROT_DIALOG_STATUS, message);
        }
    }

    DEBUG_LOG("subUiCallIndicator2Gaia Status=0x%x, call indicator: 0x%02x",
              progRun->dial_stat, msg->command);

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
    appUiPlayToneCore(app_tone_wakeup, FALSE, TRUE, NULL, 0);
}

static int16 subUiStartAssistant2Gaia(MessageId id, ProgRIPtr  progRun)
{
    if(1 != progRun->gaiaStat)
        return -1;

    appUiPlayToneCore(app_tone_wakeup, FALSE, TRUE, NULL, 0);
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

    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, 5);
    message->command = STAROT_NOTIFY_STATUS;
    appUIGetPowerInfo(progRun, message->payload);
    message->payload[3] = appUIGetPositionInfo();//位置信息
    message->payload[4] = appUIGetConnectStatusInfo();//连接信息
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
#if 0      //不应该把接近光给关掉
#ifdef HAVE_EM20168
        if(EM20168_GetStatus() == 0)
            EM20168Power(gUserParam.sensorEnable);   ///App设置是否佩戴使能
#endif
#ifdef HAVE_UCS146E0
        if(Ucs146e0_GetStatus() == 0)
            Ucs146e0Power(gUserParam.sensorEnable);
#endif
#endif
        appPeerSigTxDataCommandUi(PEERTX_CMD_SENSOR_ENB, ((gUserParam.sensorEnable == 0)? 0 : 1));
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
    UNUSED(task);

    switch(id) {
    case MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT:
        /// 需要根据实际使用的容量去计算
//        DEBUG_LOG("power precent is :%02X", ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent);
        if(progRun->iElectrity == ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent)
            break;
        progRun->iElectrity = ((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T*)message)->percent;
        if(progRun->powerflag15 == TRUE && progRun->iElectrity < 15 && (1 == progRun->bredrconnect) &&
                (appSubGetProgRun()->chargeStat == CHARGE_ST_NONE))
        {
            if(progRun->iElectrity == 1){
                appUiPlayPrompt(PROMPT_POWER_OFF);
            }
            appUiPlayPrompt(PROMPT_LOW_BATTERY);
            progRun->powerflag15 = FALSE;
        }

        if((appSubGetProgRun()->chargeStat == CHARGE_ST_CONNECT) && (0 == progRun->bredrconnect))
        {
            progRun->powerflag15 = TRUE;
        }
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
        // if(1 == gUserParam.apolloEnable)
        //     apolloWakeupPower(1);
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
        // 退出APP时，如果设置的为唤醒APP，是停止它
        //if(gUserParam.assistantType != ASSISTANT_TYPE_SYSTEM)
        //    apolloWakeupPower(0);
        appPeerSigTxDataCommandUi(PEERTX_CMD_SYNCGAIA, 0);
        break;

    case APP_BTN_DOUBLE_TAP:
        subUiKeyDoubleTap(progRun);
        break;

    case INIT_CFM:
        DEBUG_LOG("appSubUiHandleMessage INIT_CFM start");    /* Get microphone sources */
#ifdef HAVE_MAX20340
        appUiPowerSave((TRUE==max20340_GetConnect()) ? POWER_MODE_IN_CASE : POWER_MODE_OUT_CASE);
        // 现在修改位置无效，处于dfu模式，不是core状态，会忽略，现在把dfu模式，添加到core中
        max20340_notify_current_status();
#endif
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_UPGRADE);
#ifdef ENABLE_APOLLO
        register_apollo_wakeup_cb(apolloWakeupCallback);                       //注册apollo唤醒函数
#endif
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
    case HFP_CURRENT_CALLS_IND:
        subUiCallNumber2Gaia(progRun, message);
        break;
    case APP_CALL_ACTIVE:          // 拨号相关信息 接听
    case APP_CALLIN_ACT:           // 拨号相关信息 拨入
    case APP_CALLOUT_ACT:          // 拨号相关信息 拨出
    {
        //添加一个空的定时消息，如果在指定的时间内，发生了该定时器，则可以忽略hfp切换音频链路的问题
        MessageCancelAll(appGetUiTask(), APP_HFP_CHANGE_AUDIO_DIRECT_TIMEOUT);
        MessageSendLater(appGetUiTask(), APP_HFP_CHANGE_AUDIO_DIRECT_TIMEOUT, NULL, D_SEC(1));
    }
    case APP_CALLIN_INACT:         // 拨号相关信息 拨入断开
    case APP_CALLOUT_INACT:        // 拨号相关信息 拨出断开
    case APP_CALL_INACTIVE:        // 拨号相关信息 断开
        //subUiCallIndicator2Gaia(progRun, message);
        break;

    case APP_UI_HFP_STATUS_CHANGE:
        DEBUG_LOG("Call APP_UI_HFP_STATUS_CHANGE");
        subUiCallIndicator2Gaia(progRun, message);
        break;

    case GAIA_STAROT_COMMAND_IND:           // GAIA 返回过来的消息
        subUiGaiaMessage(progRun, message);
        break;

    case APP_NOTIFY_DEVICE_CON_POS:         // 向GAIA发送消息，通知当前电量、位置信息
        subUiStat2Gaia(0, progRun);
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
        appSmPairHandset();
        break;

   case APP_RESET_FACTORY: {
       //       UpgradeSMAbort();
       gProgRunInfo.needReset = TRUE;
       static int i = 0;
       i += 1;
       DEBUG_LOG("plc call reset headset : %d", i);
       if (appSmStateIsIdle(appGetState()) && APP_STATE_FACTORY_RESET != appGetState()) {
           appSmFactoryReset();
       } else {
           MessageSendLater(appGetUiTask(), APP_RESET_FACTORY, NULL, 100);
           appSmHandleResetStatusToNormal();
       }
   }
       break;
    case APP_CASE_OPEN:
        DEBUG_LOG("plc call case open");
        online_dbg_record(ONLINE_DBG_CASE_OPEN);
        appConnRulesResetEvent(RULE_EVENT_CASE_OPEN);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_CASE_OPEN);
        break;
    case APP_CASE_CLOSE:
        DEBUG_LOG("plc call case close");
        online_dbg_record(ONLINE_DBG_CASE_CLOSE);
#ifdef TWS_DEBUG
        if (appGaiaIsConnect() && !(appSmIsInDfuMode() && UpgradeInProgress())) {
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
        /// focus (plc in / in case)
        appConnRulesResetEvent(RULE_EVENT_CASE_CLOSE);
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_CASE_CLOSE);
        break;

    case APP_HFP_CHANGE_AUDIO_DIRECT_TIMEOUT:
        DEBUG_LOG("appSubUiHandleMessage APP_HFP_CHANGE_AUDIO_DIRECT_TIMEOUT");
        // 暂时不启用，在appHfpHandleHfpAudioConnectIndication中添加了两种临时状态也可以接受audio
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_HFP_REQUEST_SCO_AUDIO);
        break;

    case APP_CASE_SET_BLEINFO:              // 设置BLE信息
    case APP_CASE_SET_BTINFO:               // 盒子设置耳机经典蓝牙配对地址
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
        online_dbg_record(ONLINE_DBG_IN_CASE);
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
        online_dbg_record(ONLINE_DBG_OUT_CASE);
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
        online_dbg_record(ONLINE_DBG_IN_EAR);
        subUiEarInOutHandle(progRun, TRUE);

        DEBUG_LOG("progRun->bredrconnect =%d",progRun->bredrconnect);

        if(appDeviceIsHandsetConnected() && (appDeviceIsPeerConnected()) && (appHfpGetState() == HFP_STATE_CONNECTED_IDLE))
        {
            if((!appPeerSyncIsPeerInCase() && !appPeerSyncIsPeerInEar()) || appPeerSyncIsPeerInCase())
            {
                MessageCancelAll(&appGetUi()->task, APP_CONNECTED_HOST);
                MessageSendLater(&appGetUi()->task, APP_CONNECTED_HOST, NULL, 500);
            }
        }
//        appUiPowerSave(POWER_MODE_IN_EAR);
        break;
    case APP_PSENSOR_OUTEAR:
        online_dbg_record(ONLINE_DBG_OUT_EAR);
        subUiEarInOutHandle(progRun, FALSE);
//        appUiPowerSave(POWER_MODE_OUT_CASE);
        break;

    case APP_UPGRADE_APPLY_IND:
        DEBUG_LOG("do APP_UPGRADE_APPLY_IND");
        appUIUpgradeApplyInd();
        break;

    case APP_UPGRADE_COPY_STATUS_GRADE:
        DEBUG_LOG("do APP_UPGRADE_COPY_STATUS_GRADE");
        MessageSendLater(appGetUiTask(), APP_UPGRADE_COPY_STATUS_GRADE, NULL, D_SEC(300));
        break;

    case APP_UPGRADE_REBOOT_TIMEOUT:
        DEBUG_LOG("do APP_UPGRADE_REBOOT_TIMEOUT");
    case APP_CHECK_VERSION:
        DEBUG_LOG("do APP_CHECK_VERSION");
        appUICheckVersion();
        break;

    case APP_CHECK_PEER_FOR_UPDATE:
        DEBUG_LOG("do appUICheckPeerVersionForUpdate");
        appUICheckPeerVersionForUpdate();
        break;

    case APP_UPGRADE_COMMIT: {
        DEBUG_LOG("do APP_UPGRADE_COMMIT");
        /// 每次启动，只会触发一次提交，现在测试一下，定时发送校验版本请求
        static int onlyOne = 1;
        MessageCancelAll(appGetUiTask(), APP_CHECK_PEER_FOR_UPDATE);
        if (onlyOne > 0) {
            appUIUpgradeCommit();
            onlyOne -= 1;
        }
    }
        break;

    case APP_UPGRADE_ENTER_BY_PEER:
        DEBUG_LOG("do APP_UPGRADE_ENTER_BY_PEER");
    case APP_UPGRADE_ENTER_BY_GAIA:
        DEBUG_LOG("do APP_UPGRADE_ENTER_BY_GAIA");
        appUIUpgradeEnter();
        break;

    case APP_UPGRADE_EXIT_BY_PEER:
        DEBUG_LOG("do APP_UPGRADE_EXIT_BY_PEER");
    case APP_UPGRADE_EXIT_BY_GAIA:
        DEBUG_LOG("do APP_UPGRADE_EXIT_BY_GAIA");
        appUIUpgradeExit();
        break;

    case APP_UPGRADE_COMMIT_STATUS:
        appUIUpgradeNotifyCommitStatusInit((UI_APP_UPGRADE_COMMIT_STATUS *) message);
        break;
    case APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT:
        appUIUpgradeNotifyCommitStatusTimeOut((UI_APP_UPGRADE_COMMIT_STATUS*)message);
        break;
    case APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT_GRADE:
        appUIUpgradeNotifyCommitStatusTimeOutGrade();
        break;

    case APP_CONNECTED_HOST:
        if(appTestIsHandsetA2dpMediaConnected()){
            if(!appDeviceIsHandsetA2dpStreaming() &&
                    ((progRun->dial_stat & (DIAL_IN_ACTIVE|DIAL_OUT_ACTIVE|DIAL_ACTIVE)) == 0) && appDeviceIsHandsetConnected())
                    appUiPlayPrompt(PROMPT_CONNECTED);
        }else{
            MessageCancelAll(&appGetUi()->task, APP_CONNECTED_HOST);
            MessageSendLater(&appGetUi()->task, APP_CONNECTED_HOST, NULL, 1000);
        }
            break;

    case APP_BLE_SCANABLE_START:
        printf("appSubUiHandleMessage APP_BLE_SCANABLE_START");
        appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_BLE_CONNECTABLE_CHANGE);
        break;

    case APP_BLE_SCANABLE_TIMEOUT:
        DEBUG_LOG("appSubUiHandleMessage APP_BLE_SCANABLE_TIMEOUT");
        if (advManagerIsEnableSpecialVol()) {
            advManagerStopSpecialVol();
        }
        appGattSetAdvertisingMode(APP_ADVERT_RATE_SLOW);
        GattManagerCancelWaitForRemoteClient();
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
    progRun->caseLidOpen = UI_CASE_OPEN; /// 默认情况下，充电盒时开启状态，只有在收到明确的充电盒关闭事件，才设置为close
    appPeerVersionClearCache();

    memcpy(progRun->currVer, gFixParam.hw_ver, DEV_HWVER_LEN);
    progRun->currVer[DEV_HWVER_LEN] = ' ';
    memcpy(&progRun->currVer[DEV_HWVER_LEN+1], SystemGetCurrentSoftware(), DEV_SWVER_LEN);

    // 运行到这个地方时外设都为正常打开状态
#ifdef ENABLE_APOLLO
    apollo_sleep();
#endif

#ifdef TWS_DEBUG
    progRun->realInCase = TRUE;
    progRun->powerflag15 = TRUE;
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
    appUiHfpSaveId(number, size_number, name, size_name, 1);
    // if(appUiHfpSaveId(number, size_number, name, size_name, 1) < 0)
    //     return 0;       // 重复信息

    appSubGetProgRun()->dial_type = 1;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_NUMBER);
    memcpy(message->number, number, size_number);
    message->length = size_number;

    // 通知一下UI
    MessageSend(&appGetUi()->task, HFP_CALLER_ID_IND, message);
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

    MAKE_CALL_2_GAIA_MESSAGE(CALL_NUMBER);
    message->length = size_number;
    memcpy(message->number, number, size_number);
    MessageSend(&appGetUi()->task, HFP_CURRENT_CALLS_IND, message);

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

    DEBUG_LOG("appUiHfpCallIncomingActive");

    progRun->dial_stat = DIAL_IN_ACTIVE;
    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_IN_ACTIVE;

    MessageSend(&appGetUi()->task, APP_CALLIN_ACT, message);

    if(1 == progRun->recStat)
        MessageSend(&appGetUi()->task, STAROT_RECORD_CALLIN_STOP_STATUS_REPORT, 0);
}

/* Show HFP outcoming call */
void appUiHfpCallOutcomingActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    DEBUG_LOG("appUiHfpCallOutcomingActive");

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
    progRun->dial_stat = DIAL_OUT_ACTIVE;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_OUT_ACTIVE;

    MessageSend(&appGetUi()->task, APP_CALLOUT_ACT, message);

    if(1 == progRun->recStat)
        MessageSend(&appGetUi()->task, STAROT_RECORD_CALLOUT_STOP_STATUS_REPORT, 0);

}

/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(int16 isEnd)
{
    UNUSED(isEnd);

    DEBUG_LOG("appUiHfpCallIncomingInactive");

    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat = DIAL_IN_INACTIVE;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_IN_INACTIVE;

    MessageSend(&appGetUi()->task, APP_CALLIN_INACT, message);

    return;
}

/* Cancel HFP outcoming call */
void appUiHfpCallOutcomingInactive(int16 isEnd)
{
    UNUSED(isEnd);
    DEBUG_LOG("appUiHfpCallOutcomingInactive");

    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat = DIAL_OUT_INACTIVE;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_OUT_INACTIVE;

    MessageSend(&appGetUi()->task, APP_CALLOUT_INACT, message);

    return;
}

/* Show HFP call active */
void appUiHfpCallActive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat = DIAL_ACTIVE;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_ACTIVE;

    MessageSend(&appGetUi()->task, APP_CALL_ACTIVE, message);
}

/* Show HFP call imactive */
void appUiHfpCallInactive(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->dial_stat = DIAL_INACTIVE;

    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = DIAL_INACTIVE;

    MessageSend(&appGetUi()->task, APP_CALL_INACTIVE, message);
}

//HFP connect state
void appUiHfpConnected(unsigned cad)
{
    (void)cad;
    //当HFP连接之后，耳机在耳朵中，并且还是主耳，延时检测
    if(appSmIsInEar() && appDeviceIsHandsetConnected()){
        MessageSend(appGetUiTask(), APP_CONNECTED_HOST, NULL);
    }
}

/*EDR connect state*/
void appUiAvConnected(unsigned cad)
{
    (void)cad;
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->bredrconnect = 1;
    if(appSmIsInEar())
        apolloWakeupPower(1);
    advManagerInit();
    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
}

/*EDR disconnect state*/
void appUiAvDisconnected(void)
{
    ProgRIPtr  progRun = appSubGetProgRun();

    progRun->bredrconnect = 0;
    apolloWakeupPower(0);               // 经典蓝牙断开，关闭APO

    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
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


void appUiNotifyHtpStateChange(void) {
    DEBUG_LOG("appUiNotifyHtpStateChange %d", appHfpGetState());
    MAKE_CALL_2_GAIA_MESSAGE(CALL_INDICATOR);
    message->command = appHfpGetState();
    MessageSend(appGetUiTask(), APP_UI_HFP_STATUS_CHANGE, message);
}

/* SCO chain建立的时候，通知上层应用当前sample,rate*/
void appUiNotifyAudioSampleRate(uint16 rate, uint16 mode)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    if(progRun->gaiaStat != 1) return;

    MAKE_OBJECT(STAROT_DIALOG_STATUS_T);
    message->status = rate;
    MessageSend(appGetGaiaTask(), STAROT_DIALOG_STATUS, message);

    DEBUG_LOG("appUiNotifyAudioSampleRate rate=%d %d", rate, mode);
}

///////////////////////////////////////////////////////////////////////////////
///  盒子状态变化
///////////////////////////////////////////////////////////////////////////////
/// stat: 0：USB没有连接  1：USB连接中
void appUiCaseStatus2(int16 stat, int16 power)           // 当前USB状态
{
    ProgRIPtr  progRun = appSubGetProgRun();
    if(progRun->caseUsb != stat) {
        uint8 buff[4];
        // 获取到状态变化，同步给对方
        buff[0] = stat;
        buff[1] = (uint8)power;
        appPeerSigTxDataCommandExt(appGetUiTask(), PEERTX_CMD_SYNC_CASEST, 2, buff);
        // MessageSend
        if (progRun->caseUsb != stat) {
            progRun->caseUsb = stat;
            MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
        }
    }
}

// 对方同步过来的盒子状态
void appUiCaseStatus2FromPeer(uint8 *buff)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    if ((progRun->caseUsb != buff[0]) || (progRun->caseElectrity != buff[1])) {
        progRun->caseUsb       = buff[0];
        progRun->caseElectrity = buff[1];
        MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
    }
}


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

    if(lidOpen >= 0 && appInitCompleted()) {
        static uint8 beforeStatus = 0xFF;                   // 先设置为0xFF,这样第一次过来就能发送信息了
        if (beforeStatus != lidOpen) {
            beforeStatus = lidOpen;
            progRun->caseLidOpen = (1 == lidOpen) ? UI_CASE_OPEN : UI_CASE_CLOSE;
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

    UartPuts1("Pwr mode=", progRun->iPowerSaveMode);
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
        apolloWakeupPower(0);        // APO2
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

void appUiBatteryStat(int16 lbatt, int16 rbatt, int16 cbatt)
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

    if(iChange > 0) {
        DEBUG_LOG("appUiBatteryStat, notify APP_NOTIFY_DEVICE_CON_POS to ui");
        MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, 0);
    }
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

    appSubGetProgRun()->iElectrity = 100;           // 设置为充电满
    appSubGetProgRun()->chargeStat = CHARGE_ST_FIN;
    MessageSendLater(&appGetUi()->task, APP_CHARGE_STATUS, 0, 500);
}

uint8 appUiGetPower(void)      // 获取当前耳机电量
{
    chargerTaskData *theCharger = appGetCharger();
    uint8 value = appSubGetProgRun()->iElectrity;

    if(theCharger->is_charging)
        value |= (1<<7);               // 使用最高位表示是正在充电

    return value;
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
            ((progRun->dial_stat & (DIAL_IN_ACTIVE|DIAL_OUT_ACTIVE|DIAL_ACTIVE)) == 0)){
        if(0 == g_commuType) {
            subUiVoiceTapWakeup(progRun, FALSE);
        }
    }
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
        // 系统配置是否启动APO，与APP是否启动无关系。当前只支持主耳机的唤醒
        if(1 == gUserParam.apolloEnable && appDeviceIsHandsetConnected() && (0 == progRun->apolloWakeup) /* &&
                (1 == progRun->gaiaStat || gUserParam.assistantType == ASSISTANT_TYPE_SYSTEM) */){
            progRun->apolloWakeup = 1;
            OperatorFrameworkEnable(MAIN_PROCESSOR_ON);    // 前,否则有时电压上不来
            appSubUISetMicbias(TRUE);
#ifdef ENABLE_APOLLO
            apollo_evoke();
#endif
        }
    }
    else {
        if(1 == progRun->apolloWakeup) {          // 当前已经启动
#ifdef ENABLE_APOLLO
            apollo_sleep();                  // APO2,可能不成功
#endif
            appSubUISetMicbias(FALSE);
            OperatorFrameworkEnable(MAIN_PROCESSOR_OFF);
            progRun->apolloWakeup = 0;
        }
    }
}

bool appUICaseIsOpen(void) {
    ProgRIPtr  progRun = appSubGetProgRun();
    return (progRun->caseLidOpen == UI_CASE_OPEN ? TRUE : FALSE);
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
    uint8 chargerFlag = 0X80, illegalBattery = 0XFF;
    if (appConfigIsLeft()) {
        arr[0] = (appUICurrentIsCharger() ? chargerFlag : 0X00) | (uint8)progRun->iElectrity;
        arr[1] = illegalBattery;
        if (appDeviceIsPeerConnected()) {
            arr[1] = (appUIPeerIsCharger() ? chargerFlag : 0X00) | (uint8)progRun->peerElectrity;//对方电量
        }
    } else {
        arr[0] = illegalBattery;
        if (appDeviceIsPeerConnected()) {
            arr[0] = (appUIPeerIsCharger() ? chargerFlag : 0X00) | (uint8)progRun->peerElectrity;//对方电量
        }
        arr[1] = ((appUICurrentIsCharger() ? chargerFlag : 0X00)) | (uint8)progRun->iElectrity;
    }

    if (appPeerSyncIsPeerInCase() || appSmIsInCase()) {
        arr[2] = (uint8)progRun->caseElectrity | (progRun->caseUsb ? chargerFlag : 0X00); //盒子电量 充电，最高位为1
    } else {
        arr[2] = illegalBattery; /// 任何一只耳机再充电盒中，都可以读取充电盒状态数据
    }
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
        OperatorFrameworkEnable(MAIN_PROCESSOR_ON);    // 需要放在SetMicbias之前,否则有时电压上不来
        appSubUISetMicbias(TRUE);
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
    DEBUG_LOG("MAC ADDRESS : %02X%02X%02X%02X%02X%02X", addrbuf[0], addrbuf[1], addrbuf[2], addrbuf[3], addrbuf[4], addrbuf[5],);
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

void appPeerVersionSet(uint8* buffer) {
    memcpy(gProgRunInfo.peerVer, buffer, sizeof(gProgRunInfo.peerVer));
}

bool appPeerVersionIsEmpty(void) {
    bool isEmpty = TRUE;
    int i = 0;
    for (i = 0; i < sizeof(gProgRunInfo.peerVer); ++i) {
        if ((gProgRunInfo.peerVer)[i] != 0XFF) {
            isEmpty = FALSE;
            break;
        }
    }
    return isEmpty;
}

uint8* appPeerVersionGet(void) {
    return gProgRunInfo.peerVer;
}

uint8* appCurrVersionGet(void) {
    return gProgRunInfo.currVer;
}


bool appPeerVersionSyncStatusIsComplete(void) {
    return (gProgRunInfo.peerVerSyncStatus == (uint8)PeerVersionSyncStatusComplete);
}

void appPeerVersionSyncStatusSet(uint8 status) {
    gProgRunInfo.peerVerSyncStatus |= status;
}

void appPeerVersionSyncStatusClean(uint8 status) {
    gProgRunInfo.peerVerSyncStatus ^= status;
}

void appPeerVersionSyncSent(void) {
    if (ParamUsingSingle()) {
        appPeerVersionSyncStatusSet(PeerVersionSyncStatusSent | PeerVersionSyncStatusRecv);
    } else {
        appPeerSigTxSyncVersionReq(appGetUiTask());
        appPeerVersionSyncStatusSet(PeerVersionSyncStatusSent);
    }
}

void appPeerVersionClearCache(void) {
    gProgRunInfo.peerVerSyncStatus = 0;
    int i = 0;
    for (i = 0; i < DEV_HWSWVER_LEN; ++i) {
        gProgRunInfo.peerVer[i] = 0XFF;
    }
}

bool appPeerVersionSyncStatusHaveSent(void) {
    return (gProgRunInfo.peerVerSyncStatus & PeerVersionSyncStatusSent) > 0;
}

static void appUIUpgradeApplyInd(void) {
    DEBUG_LOG("appUIUpgradeApplyInd parse, now sync version to peer earbuds");
    // 使用定时器，如果MESSAGE_IMAGE_UPGRADE_COPY_STATUS（耗时操作）收到了，说明文件拷贝已经结束了
    if (MessageCancelAll(appGetUiTask(), APP_UPGRADE_COPY_STATUS_GRADE) <= 0) {
        // copy 还没有完成，不需要及时修改版本信息
        DEBUG_LOG("appUIUpgradeApplyInd, copy not over, so defer this operation");
        MessageSendLater(appGetUiTask(), APP_UPGRADE_APPLY_IND, NULL, 100);
        return;
    }
    appUITempSetVersionToMemory(gProgRunInfo.tempCurrentVer);

    // 如果双耳模式，查看另一只耳机版本。如果另一只耳机已经升级成功，向另一只耳机发送重启命令，当前耳机在收到重启命令的确认时重启，并设置定时器
    // 如果单耳模式，直接重启
    if (ParamUsingSingle()) {
        UpgradeApplyResponse(0);
    } else {
        // 同步版本到另一只耳机
        gProgRunInfo.upgradeNeedReboot = TRUE;
        appPeerVersionSyncStatusSet(0);
        appPeerVersionSyncSent();
        const int versionSame = 2;
        if (versionSame != SystemCheckMemoryVersion()) {
            DEBUG_LOG("SystemCheckVersionWithPeer is not same, so need exit dfu mode");
            appSmHandleDfuEnded(TRUE);
        } else {
            DEBUG_LOG("SystemCheckVersionWithPeer is same, now need think how reboot");
            /// 可以执行重启，添加定时器，在版本同步确认的时候，去重新启动
            // todo 此处需要考虑如果两只耳机连接不上，会发生什么事情
            MessageSendLater(appGetUiTask(), APP_UPGRADE_REBOOT_TIMEOUT, NULL, D_SEC(5));
        }
    }
}

static void appUICheckVersion(void) {
    DEBUG_LOG("enter appUICheckVersion");
    appConnRulesSetEvent(appGetSmTask(), RULE_EVENT_BLE_CONNECTABLE_CHANGE);
    if (gProgRunInfo.upgradeNeedReboot) {
        const int versionSame = 2;
        if (versionSame == SystemCheckMemoryVersion()) {
            DEBUG_LOG("appUICheckVersion SystemCheckVersionWithPeer is same, so need reboot self");
            gProgRunInfo.upgradeNeedReboot = FALSE;
            UpgradeApplyResponse(0);
        }
    }
}

extern void UpgradeCommitNewImage(void);
extern void UpgradeRevertNewImage(void);

static int haveCallappUICheckPeerVersionForUpdate = 0;
static void appUICheckPeerVersionForUpdate(void) {
    haveCallappUICheckPeerVersionForUpdate += 1;
    DEBUG_LOG("call appUICheckPeerVersionForUpdate");
    if (ParamUsingSingle()) {
        /// 提交版本信息
        UpgradeCommitNewImage();
    } else {
        /// note:只有重启才会进入该段code，所以不用重置(不管成功还是失败)
        static int count = 1000;
        if (count >= 0) {
            DEBUG_LOG("appUICheckPeerVersionForUpdate, resend appUICheckPeerVersionForUpdate, count is :%d", count);
            /// 重新发送同步version信息
            CheckVersion checkVersion;
            memcpy(checkVersion.softwareVersion, SystemGetCurrentSoftware(), DEV_SWVER_LEN);
            appPeerSigTxUpgradeCheckVersionReq(appGetUiTask(), &checkVersion);
            count -= 1;
        } else {
            /// 回滚版本
            DEBUG_LOG("appUICheckPeerVersionForUpdate, revert image, count is :%d", count);
            UpgradeRevertNewImage();
        }
    }
}

void appUITempSetVersionToMemory(uint8* ptr) {
    for (int i = 0; i < DEV_SWVER_LEN; ++i) {
        gProgRunInfo.currVer[DEV_HWVER_LEN + 1 + i] = ptr[i];
    }
    appPeerVersionSyncSent();
}

void appUIConvertTempVersionToMemory(void) {
    memcpy(gProgRunInfo.currVer + DEV_HWVER_LEN, SystemGetCurrentSoftware(), DEV_SWVER_LEN);
    appPeerVersionSyncSent();
}

static void appUIUpgradeCommit(void) {
    UpgradeCommitNewImage();
}

static void appUIUpgradeEnter(void) {
    DEBUG_LOG("call appUIUpgradeEnter");
    gProgRunInfo.canContinueUpgrade = TRUE;
    appSmEnterDfuMode();
}

void appUIUpgradeExit(void) {
    DEBUG_LOG("call appUIUpgradeExit");
    gProgRunInfo.canContinueUpgrade = FALSE;
    appSmHandleDfuEnded(TRUE);
}

bool appUICanContinueUpgrade(void) {
    return gProgRunInfo.canContinueUpgrade;
}

void appUICancelAllUpgradeTime(void) {
    MessageCancelAll(appGetUiTask(), APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT);
    MessageCancelAll(appGetUiTask(), APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT_GRADE);
}



static void appUIUpgradeNotifyCommitStatusInit(UI_APP_UPGRADE_COMMIT_STATUS* message) {
    MessageSendLater(appGetUiTask(), APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT_GRADE, NULL, D_SEC(5 * 60));
    appUIUpgradeNotifyCommitStatusDo(message);
}

static void appUIUpgradeNotifyCommitStatusDo(UI_APP_UPGRADE_COMMIT_STATUS* message) {

    { // send to gaia
        GAIA_STAROT_MESSAGE_T *msg = (GAIA_STAROT_MESSAGE_T *) PanicUnlessMalloc(sizeof(GAIA_STAROT_MESSAGE_T));
        msg->command = STAROT_UI_NOTIFY_COMMIT_STATUS;
        msg->payloadLen = 1;
        msg->payload[0] = message->status;
        MessageSend(appGetGaiaTask(), GAIA_STAROT_COMMAND_IND, msg);
    }
    { // set timeout for resend
        UI_APP_UPGRADE_COMMIT_STATUS *msg = (UI_APP_UPGRADE_COMMIT_STATUS *) PanicUnlessMalloc(
                sizeof(UI_APP_UPGRADE_COMMIT_STATUS));
        msg->status = message->status;
        MessageSendLater(appGetUiTask(), APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT, msg, D_SEC(5));
    }
}

static void appUIUpgradeNotifyCommitStatusTimeOut(UI_APP_UPGRADE_COMMIT_STATUS* message) {
    appUIUpgradeNotifyCommitStatusDo(message);
}

static void appUIUpgradeNotifyCommitStatusTimeOutGrade(void) {
    DEBUG_LOG("call appUIUpgradeNotifyCommitStatusTimeOutGrade");
    MessageCancelAll(appGetUiTask(), APP_UPGRADE_NOTIFY_COMMIT_TIMEOUT);
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

// region SN同步

void appUISetPeerSnDetail(uint8* peerSn, uint8 len) {
    DEBUG_LOG("appUISetPeerSnDetail");
    memcpy(gProgRunInfo.peerSn, peerSn, len > DEV_SN_LEN ? DEV_SN_LEN : len);
}

void appUISetPeerSnStatus(uint8 status) {
    DEBUG_LOG("appUISetPeerSnStatus before %d, status : %d", gProgRunInfo.peerSnSyncStatus, status);
    gProgRunInfo.peerSnSyncStatus |= status;
    DEBUG_LOG("appUISetPeerSnStatus after %d, status : %d", gProgRunInfo.peerSnSyncStatus, status);
}

bool appUIGetPeerSnStatusIsComplete(void) {
    return gProgRunInfo.peerSnSyncStatus == PEER_SN_SYNC_COMPLETE;
}

void appUIUnSetPeerSnStatus(uint8 status) {
    DEBUG_LOG("appUIUnSetPeerSnStatus before is :%02X", gProgRunInfo.peerSnSyncStatus);
    gProgRunInfo.peerSnSyncStatus ^= status;
    DEBUG_LOG("appUIUnSetPeerSnStatus after is :%02X", gProgRunInfo.peerSnSyncStatus);
}

bool appUIGetPeerSnStatusIsHaveSent(void) {
    return (gProgRunInfo.peerSnSyncStatus & PEER_SN_SYNC_SENT) > 0;
}

const uint8 *appUIGetPeerSnDetail(void) {
    return gProgRunInfo.peerSn;
}

void appUIClearPeerSnStatus(void) {
    DEBUG_LOG("appUIClearPeerSnStatus");
    gProgRunInfo.peerSnSyncStatus = 0;
}

// endregion


// region 工具类方法

bool appUICurrentIsCharger(void) {
    chargerTaskData *theCharger = appGetCharger();
    return theCharger->is_charging > 0;
}

bool appUIPeerIsCharger(void) {
    peerSyncTaskData* ps = appGetPeerSync();
    return (ps->peer_battery_level & 0X8000) > 0;
}

// endregion
