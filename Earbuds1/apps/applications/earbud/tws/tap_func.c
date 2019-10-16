#include "bma400.h"
#include "lis2dw12.h"
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <hydra_macros.h>
#include <input_event_manager.h>

typedef struct tagSHELLCMDINFO {
    TaskData       task;
}tapfuncInfoTask;
static tapfuncInfoTask *pTapfuncTask = NULL;

#ifdef HAVE_BMA400
void tap_itr_handle_bma400(void)
{
    uint8 value_a[3];
    bitserial_handle handle;
    handle = BMA400Enable();
    BMA400ReadRegister(handle, 0x0e, value_a+0);
    BMA400ReadRegister(handle, 0x0f, value_a+1);
    BMA400ReadRegister(handle, 0x10, value_a+2);
    //printf("reg0x0e=0x%x, reg0x0f=0x%x, reg0x10=0x%x\n",value_a[0], value_a[1], value_a[2]);
#ifdef MUTI_TAP
    uint32 time;
    if(value_a[1] & 0x4){
        time = VmGetClock();
        printf("single tap, time = %d\n", time);
    }
#endif
#ifndef ONLY_DOUBLE_TAP
    if(value_a[1] & 0x4){
        printf("single tap\n");
        MessageSend(&(appGetUi()->task), 1000, NULL);
    }
#endif
#ifndef MUTI_TAP
    if(value_a[1] & 0x8){
        printf("double tap\n");
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
//    printf("reg 0x37=0x%x, 0x38=0x%x, 0x39=0x%x, 0x3a=0x%x, 0x3b=0x%x\n",
//           value_arr[0], value_arr[1], value_arr[2], value_arr[3], value_arr[4]);
    if(value_arr[0] & 0x10){
        printf("double tap\n");
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
            printf("id=%d(0x%x\n", id, id);
            break;
    }
}

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
}


