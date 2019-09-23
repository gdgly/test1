#ifdef CONFIG_STAROT
#include <panic.h>
#include <ps.h>
#include <boot.h>
#include <input_event_manager.h>

#include "av_headset.h"
#include "av_headset_ui.h"
#include "av_headset_sm.h"
#include "av_headset_hfp.h"
#include "av_headset_power.h"
#include "av_headset_log.h"
#include "sub_ui.h"

#include <pmalloc.h>
ProgRunInfo gProgRunInfo;


// Task HFP 通过这个函数来发送消息
static int16 subUiHfpCallerId(void)
{
    DEBUG_LOG("HFP CALL in");

    return 0;
}

// UI 自定义的相关消息
void appSubUiHandleMessage(Task task, MessageId id, Message message)
{
    (void)task, (void)message;
    switch(id) {
    // 拨号、电话相关的消息
    case HFP_CALLER_ID_IND:
        subUiHfpCallerId();
        break;

    // 盒子发送相关的命令操作
    case APP_CASE_REPORT_VERSION:           // 盒子硬件版本信息等
    case APP_CASE_REPORT_INFO:              // 盒子报告当前信息
    case APP_CASE_SET_BLEINFO:              // 设置BLE信息
    case APP_CASE_SET_BTINFO:               // 盒子设置耳机经典蓝牙配对地址
        break;
    }
}

// HFP TASK调用，新的号码拨入
int16 appUiHfpCallerId(uint8 *number, uint16 size_number, uint8 *name, uint16 size_name)
{
    ProgRIPtr  progRun = &gProgRunInfo;
    CallIPtr   pCall;

//    if(1) return 0;

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

/* Show HFP incoming call */
void appUiHfpCallIncomingActive(void)
{
}

/* Cancel HFP incoming call */
void appUiHfpCallIncomingInactive(void)
{
}

/* Show HFP call active */
void appUiHfpCallActive(void)
{
}

/* Show HFP call imactive */
void appUiHfpCallInactive(void)
{

}

#endif
