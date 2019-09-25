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

    DEBUG_LOG("\nHFP CALL Status=0x%x", progRun->dial_stat);
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
    ProgRIPtr  progRun = &gProgRunInfo;
    (void)task, (void)message;

    switch(id) {
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
    case APP_CASE_REPORT_INFO:              // 盒子报告当前信息
    case APP_CASE_SET_BLEINFO:              // 设置BLE信息
    case APP_CASE_SET_BTINFO:               // 盒子设置耳机经典蓝牙配对地址
        break;
    }
}

static bool HfpCallerIsSame(uint8 *number, uint16 size_number)
{
    ProgRIPtr  progRun = &gProgRunInfo;
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
    ProgRIPtr  progRun = &gProgRunInfo;
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
    ProgRIPtr  progRun = &gProgRunInfo;

    progRun->callIndex = MAX_CALLIN_INFO;  // 设置为无效值

    progRun->dial_stat  &= ~DIAL_ST_INACT;
    progRun->dial_stat  |= DIAL_ST_IN;

    MessageSend(&appGetUi()->task, APP_CALLIN_ACT, 0);
}

/* Show HFP outcoming call */
void appUiHfpCallOutcomingActive(void)
{
    ProgRIPtr  progRun = &gProgRunInfo;

    progRun->dial_stat  &= ~DIAL_ST_INACT;
    progRun->dial_stat  |= DIAL_ST_OUT;

    MessageSend(&appGetUi()->task, APP_CALLOUT_ACT, 0);
}

/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = &gProgRunInfo;

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_IN | DIAL_ST_ACT);

    MessageSend(&appGetUi()->task, APP_CALLIN_INACT, 0);
}

/* Cancel HFP outcoming call */
void appUiHfpCallOutcomingInactive(int16 isEnd)
{
    ProgRIPtr  progRun = &gProgRunInfo;

    if(isEnd)
        progRun->dial_stat  &= ~(DIAL_ST_OUT | DIAL_ST_ACT);

    MessageSend(&appGetUi()->task, APP_CALLOUT_INACT, 0);
}

/* Show HFP call active */
void appUiHfpCallActive(void)
{
    ProgRIPtr  progRun = &gProgRunInfo;

    progRun->dial_stat  |= DIAL_ST_ACT;

    MessageSend(&appGetUi()->task, APP_CALL_ACTIVE, 0);
}

/* Show HFP call imactive */
void appUiHfpCallInactive(void)
{
    ProgRIPtr  progRun = &gProgRunInfo;

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
    ProgRIPtr  progRun = &gProgRunInfo;

    (void)keyDown, (void)keyLong, (void)iElectrity;

    if(lidOpen >= 0)
        progRun->caseLidOpen = (1 == lidOpen) ? 1 : 0;



}

void appUiCaseVersion(uint16 hwVer, uint16 swVer)
{
    (void)hwVer, (void)swVer;

}



#endif
