#include "em20168.h"
#ifdef HAVE_EM20168

em20168_str em20168_init_array[] = {
#if 0
    {0x01, 0x80},
    {0x0b, 0x80},
    {0x0c, 0xc0},
    {0x0d, 0x3c},//25ms
    //{0x0d, 0x5c},//400ms
    {0x0e, 0x71},
    {0x24, 0x30},
    {0x3, EM20168_LOW_VALUE_L},
    {0x4, EM20168_LOW_VALUE_H},
    {0x5, EM20168_HIGH_VALUE_L},
    {0x6, EM20168_HIGH_VALUE_H},
#endif
    {0x26, 0x00},
    {0x01, 0x80},
    {0x0b, 0x80},
    {0x0c, 0x80},
    //{0x0d, 0xb6},
    {0x0d, 0xc6},
    //{0x0e, 0x7f},
    {0x0e, 0x72},
    //{0x24, 0x0b},
    {0x24, 0x00},
    {0x12, 0x96},
    //{0x13, 0xc8},
    //{0x1b, 0x80},
    {0x1b, 0xc0},
    {0x27, 0x00},
    {0x1d, 0xf0},
    {0x33, 0x68},
    {0x34, 0x00},
    {0x25, 0x00},
    {0x3, EM20168_LOW_VALUE_L},
    {0x4, EM20168_LOW_VALUE_H},
    {0x5, EM20168_HIGH_VALUE_L},
    {0x6, EM20168_HIGH_VALUE_H},
};

em20168_str em20168_read_array[] = {
    {0x01, 0x0},
    {0x0b, 0x0},
    {0x0c, 0x0},
    {0x0d, 0x0},
    {0x0e, 0x0},
    {0x13, 0x0},
    {0x24, 0x0},
};

/*! \brief Read a register from the proximity sensor */
bool EM20168ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
{
    bitserial_result result;
    /* First write the register address to be read */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            &reg, 1,
                            BITSERIAL_FLAG_BLOCK);
    if (result == BITSERIAL_RESULT_SUCCESS){
        /* Now read the actual data in the register */
        result = BitserialRead(handle,
                                BITSERIAL_NO_MSG,
                                value, 1,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        //printf("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

/*! \brief Write to a proximity sensor register */
bool EM20168WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
{
    bitserial_result result;
    uint8 command[2] = {reg, value};

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            command, 2,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        printf("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bitserial_handle EM20168Enable(void)
{
    uint16 bank;
    uint32 mask;

    //printf("EM20168Enable");
    bank = PIO2BANK(EM20168_ITR_PIN);
    mask = PIO2MASK(EM20168_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

#ifdef EM20168_KEY_ITR_TEST
    bank = PIO2BANK(EM20168_KEY_ITR_PIN);
    mask = PIO2MASK(EM20168_KEY_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
#endif

    return hwi2cOpen(EM20168_I2C_ADDR, EM20168_I2C_FREQ);
}

void EM20168Disable(bitserial_handle handle)
{
    //printf("EM20168Disable");
    hwi2cClose(handle);
}

int EM20168Power(bool isOn)
{
    int ret;
    uint8 value;
    bitserial_handle handle;
    handle = EM20168Enable();
    EM20168ReadRegister(handle, 0x00, &value);
    EM20168ReadRegister(handle, 0x00, &value);
    if(isOn){
        ret = EM20168WriteRegister(handle, 0x01, 0x80);
    }else{
        ret = EM20168WriteRegister(handle, 0x01, 0);
    }
    EM20168Disable(handle);
    return ret;
}

void EM20168_itr_read_reg(Task task, MessageId id, Message msg)
{
    (void)id;(void)msg;(void)task;
    bitserial_handle handle;
    uint8 value;
    unsigned short em20168_ps0_value;
    proximityTaskData *prox = appGetProximity();
    handle = EM20168Enable();

    EM20168ReadRegister(handle, 0x21, &value);
    em20168_ps0_value = value << 8;
    EM20168ReadRegister(handle, 0x20, &value);
    em20168_ps0_value += value;
#ifndef EM20168_SEND_MSG
    DEBUG_LOG("EM20168 reg = 0x%x\n\n", em20168_ps0_value);
#endif
    EM20168WriteRegister(handle, 2, 0);

    if(em20168_ps0_value >= EM20168_HIGH_VALUE &&
            (prox->state->proximity != proximity_state_in_proximity) ){
        prox->state->proximity = proximity_state_in_proximity;
#ifndef EM20168_SEND_MSG
        DEBUG_LOG("in ear\n\n");
#else
        if (NULL != prox->clients)
            appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_IN_PROXIMITY);
#endif
    }
    if(em20168_ps0_value <= EM20168_LOW_VALUE &&
            (prox->state->proximity == proximity_state_in_proximity) ){
        prox->state->proximity = proximity_state_not_in_proximity;
#ifndef EM20168_SEND_MSG
        DEBUG_LOG("out ear\n\n");
#else
        if (NULL != prox->clients)
            appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY);
#endif
    }

    EM20168Disable(handle);
}

void EM20168_itr_handler(Task task, MessageId id, Message msg)
{
    (void)task;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = EM20168_ITR_PIN;
    pin = PIO2MASK(pin);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                EM20168_itr_read_reg(task, id, msg);
            }
            break;
        default:
            printf("id=%d(0x%x\n", id, id);
            break;
    }
}

#ifdef EM20168_KEY_ITR_TEST
void EM20168_keytest_itr_handler(Task task, MessageId id, Message msg)
{
    (void)task;uint8 value;
    unsigned short em20168_ps0_value;
    bitserial_handle handle;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = EM20168_KEY_ITR_PIN;
    pin = PIO2MASK(pin);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                handle = EM20168Enable();
                EM20168ReadRegister(handle, 0x00, &value);
                EM20168ReadRegister(handle, 0x00, &value);
                printf("EM20168 id = 0x%x\n\n", value);

                EM20168ReadRegister(handle, 0x21, &value);
                em20168_ps0_value = value << 8;
                EM20168ReadRegister(handle, 0x20, &value);
                em20168_ps0_value += value;
                printf("EM20168 reg = 0x%x\n\n", em20168_ps0_value);
                EM20168WriteRegister(handle, 2, 0);
                EM20168Disable(handle);
            }
            break;
        default:
            printf("id=%d(0x%x\n", id, id);
            break;
    }
}
#endif

typedef struct tagSHELLCMDINFO {
    TaskData       task;
}EM20168InfoTask;
static EM20168InfoTask *EM20168Task = NULL;
#ifdef EM20168_KEY_ITR_TEST
static EM20168InfoTask *ProximitykeyTask = NULL;
#endif

static void delay_ms(int time_ms)
{
    uint32 time_now = VmGetTimerTime();
    uint32 time_end = time_now + time_ms*1000;
    while(1){
        time_now = VmGetTimerTime();
        if(time_now > time_end){
            break;
        }
    }
}

void EM20168_init(void)
{
    bitserial_handle handle;
    uint8 value;
    uint8 i;
    value = 0;
#ifdef EM20168_CAL_OFFSET_VALUE
    uint8 offset=0;
    uint8 ps_l=0;
    uint8 ps_h=0;
    uint16 ps_data=0;
#endif

    handle = EM20168Enable();
    PanicFalse(handle != BITSERIAL_HANDLE_ERROR);

    EM20168ReadRegister(handle, 0x00, &value);
    EM20168ReadRegister(handle, 0x00, &value);
    if(value == 0x37)
        printf("em20168 id = 0x%x\n", value);
    else{
        EM20168Disable(handle);
        printf("em20168 read id error!\n");
        return;
    }

    EM20168WriteRegister(handle,0x14,0x00);
    delay_ms(100);

    for(i=0; i<ARRAY_DIM(em20168_init_array); i++){
        EM20168WriteRegister(handle,
                em20168_init_array[i].reg,
                em20168_init_array[i].value);
    }

    for(i=0; i<ARRAY_DIM(em20168_init_array); i++){
        EM20168ReadRegister(handle, em20168_init_array[i].reg, &value);
        printf("reg 0x%x = 0x%x\n", em20168_init_array[i].reg, value);
    }

#ifdef EM20168_CAL_OFFSET_VALUE
    for(i=0; i<5; i++){
        printf("##############%d\n",i);
        delay_ms(1000);
    }
    for(i=0;i<=128;i++){
        delay_ms(100);
        EM20168ReadRegister(handle, 0x21, &ps_h);
        EM20168ReadRegister(handle, 0x20, &ps_l);
        ps_data = (ps_h <<8) | ps_l;
        if(ps_data != 0){
            offset +=1;
            EM20168WriteRegister(handle, 0x24, offset);
            printf("20168 catching offset EM20168 reg = 0x%x 0x%x \n\n", ps_data,offset);
        }else{
            printf("20168 steady mode EM20168 reg = 0x%x 0x%x \n\n", ps_data,offset);
            break;
        }
    }
#endif

    EM20168WriteRegister(handle, 2, 0);//clear itr

    EM20168Task = PanicUnlessNew(EM20168InfoTask);
    memset(EM20168Task, 0, sizeof(EM20168Task));
    EM20168Task->task.handler = EM20168_itr_handler;
    InputEventManagerRegisterTask(&EM20168Task->task, EM20168_ITR_PIN);

#ifdef EM20168_KEY_ITR_TEST
    ProximitykeyTask = PanicUnlessNew(EM20168InfoTask);
    memset(ProximitykeyTask, 0, sizeof(ProximitykeyTask));
    ProximitykeyTask->task.handler = EM20168_keytest_itr_handler;
    InputEventManagerRegisterTask(&ProximitykeyTask->task, EM20168_KEY_ITR_PIN);
#endif

    EM20168Disable(handle);
}


#ifdef INCLUDE_PROXIMITY
bool appProximityClientRegister(Task task)
{
    proximityTaskData *prox = appGetProximity();
    if (NULL == prox->clients)
    {
        prox->state = PanicUnlessNew(proximityState);
        prox->state->proximity = proximity_state_unknown;
        prox->clients = appTaskListInit();
    }
    /* Send initial message to client */
    switch (prox->state->proximity)
    {
        case proximity_state_in_proximity:
            MessageSend(task, PROXIMITY_MESSAGE_IN_PROXIMITY, NULL);
            break;
        case proximity_state_not_in_proximity:
            MessageSend(task, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY, NULL);
            break;
        case proximity_state_unknown:
            MessageSend(task, PROXIMITY_MESSAGE_IN_PROXIMITY, NULL);
            break;
        default:
            /* The client will be informed after the first interrupt */
            break;
    }

    return appTaskListAddTask(prox->clients, task);
}

void appProximityClientUnregister(Task task)
{
    proximityTaskData *prox = appGetProximity();
    appTaskListRemoveTask(prox->clients, task);
    if (0 == appTaskListSize(prox->clients))
    {
        appTaskListDestroy(prox->clients);
        prox->clients = NULL;
        free(prox->state);
        prox->state = NULL;

        PanicFalse(prox->handle != BITSERIAL_HANDLE_ERROR);
        /* Unregister for interrupt events */
        InputEventManagerUnregisterTask(&prox->task, EM20168_ITR_PIN);

        /* Reset into lowest power mode in case the sensor is not powered off. */
        prox->handle = BITSERIAL_HANDLE_ERROR;
    }
}
#endif

#endif

