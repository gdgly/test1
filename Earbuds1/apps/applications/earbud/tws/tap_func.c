#include "bma400.h"
#include "lis2dw12.h"
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <hydra_macros.h>
#include <input_event_manager.h>

//#define TIME_READ_LIS2DW12_REG

typedef struct tagSHELLCMDINFO {
    TaskData       task;
}tapfuncInfoTask;
static tapfuncInfoTask *pTapfuncTask = NULL;
#ifdef TIME_READ_LIS2DW12_REG
static tapfuncInfoTask *pTimefuncTask = NULL;
#endif

#ifdef HAVE_BMA400
void tap_itr_handle_bma400(void)
{
    uint8 value_a[3];
    bitserial_handle handle;
    handle = BMA400Enable();
    BMA400ReadRegister(handle, 0x0e, value_a+0);
    BMA400ReadRegister(handle, 0x0f, value_a+1);
    BMA400ReadRegister(handle, 0x10, value_a+2);
    //DEBUG_LOG("reg0x0e=0x%x, reg0x0f=0x%x, reg0x10=0x%x\n",value_a[0], value_a[1], value_a[2]);
#ifdef MUTI_TAP
    uint32 time;
    if(value_a[1] & 0x4){
        time = VmGetClock();
        DEBUG_LOG("single tap, time = %d\n", time);
    }
#endif
#ifndef ONLY_DOUBLE_TAP
    if(value_a[1] & 0x4){
        DEBUG_LOG("single tap\n");
        MessageSend(&(appGetUi()->task), 1000, NULL);
    }
#endif
#ifndef MUTI_TAP
    if(value_a[1] & 0x8){
        DEBUG_LOG("double tap\n");
        //MessageSend(&(appGetUi()->task), 1000, NULL);
        MessageSend(&(appGetUi()->task), 1006, NULL);
        CONN_RULES_BLE_CONNECTION_UPDATE_T *msg_value = PanicUnlessNew(CONN_RULES_BLE_CONNECTION_UPDATE_T);
        msg_value->enable = TRUE;
        MessageSend(&(appGetSm()->task), CONN_RULES_BLE_CONNECTION_UPDATE, msg_value);
    }
#endif
    BMA400Disable(handle);
}
#endif

#ifdef HAVE_LIS2DW12
void tap_itr_handle_lis2dw12(void)
{
    //uint8 value;
    uint8 value_arr[16];
    bitserial_handle handle;
    handle = lis2dw12Enable();
    lis2dw12ReadRegister_withlen(handle, 0x37, value_arr, 5);
//    DEBUG_LOG("reg 0x37=0x%x, 0x38=0x%x, 0x39=0x%x, 0x3a=0x%x, 0x3b=0x%x\n",
//           value_arr[0], value_arr[1], value_arr[2], value_arr[3], value_arr[4]);

    if(0 == lis2dw12Runing) {  // poweroff 后不需要分发消息
        DEBUG_LOG("tap_itr_handle_lis2dw12=0x%X but Poweroff", value_arr[0]);
        lis2dw12Disable(handle);
    }

    if(value_arr[0] & 0x10){
        DEBUG_LOG("double tap\n");
        if(4 == g_commuType){
            char* buff = "check TAP SUSS";
            CommpcMessage((uint8*)buff, 15);
            return;       // 不发送消息给主UI
        }
        MessageSend(appGetUiTask(), APP_BTN_DOUBLE_TAP, NULL);
    }
    lis2dw12Disable(handle);
    return;
}
#endif

void tap_itr_handler(Task task, MessageId id, Message msg)
{
    //(void)id;(void)msg;(void)task;
    (void)task;
    unsigned long pin=0, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
#ifdef HAVE_BMA400
    pin = BMA400_ITR_PIN;
#endif
#ifdef HAVE_LIS2DW12
    pin = LIS2DW12_ITR_PIN;
#endif
    pin = PIO2MASK(pin);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
#ifdef HAVE_BMA400
                    tap_itr_handle_bma400();
#endif
#ifdef HAVE_LIS2DW12
                    tap_itr_handle_lis2dw12();
#endif
            }
            break;
        default:
            DEBUG_LOG("id=%d(0x%x\n", id, id);
            break;
    }
}

#ifdef TIME_READ_LIS2DW12_REG
#define MESSAGE_TIME_TRIGGER 1
static void tap_time_handle_msg(Task task, MessageId id, Message message)
{
    (void)message;(void)task;
    switch (id)
    {
        case MESSAGE_TIME_TRIGGER:
            tap_itr_handle_lis2dw12();
            //DEBUG_LOG("1s read lis2dw12 reg\n");
            MessageSendLater(&pTimefuncTask->task,
                             MESSAGE_TIME_TRIGGER, NULL,
                             1000);
        break;
    }
}
#endif

void tap_func_init(void)
{
    pTapfuncTask = PanicUnlessNew(tapfuncInfoTask);
    memset(pTapfuncTask, 0, sizeof(tapfuncInfoTask));
#ifdef HAVE_BMA400
    BMA400_init();
    pTapfuncTask->task.handler = tap_itr_handler;
    InputEventManagerRegisterTask(&pTapfuncTask->task, BMA400_ITR_PIN);
#endif
#ifdef HAVE_LIS2DW12
    lis2dw12_init();
    pTapfuncTask->task.handler = tap_itr_handler;
    InputEventManagerRegisterTask(&pTapfuncTask->task, LIS2DW12_ITR_PIN);
#endif
#ifdef TIME_READ_LIS2DW12_REG
    pTimefuncTask= PanicUnlessNew(tapfuncInfoTask);
    memset(pTimefuncTask, 0, sizeof(tapfuncInfoTask));
    pTimefuncTask->task.handler = tap_time_handle_msg;
    MessageSendLater(&pTimefuncTask->task,
                     MESSAGE_TIME_TRIGGER, NULL,
                     5000);
#endif
}


