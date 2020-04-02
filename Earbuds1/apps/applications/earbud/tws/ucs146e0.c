#include "ucs146e0.h"
#ifdef HAVE_UCS146E0

static uint8  _ucs146e0Runing = 0;              // poweroff后，设置为 0, 启动运行为1
static uint8  _ucs146e0StatInOut = 0xFF;           // 0xFF unknown, 0:in ear, 1:out ear

static void Ucs146e0_itr_handler(Task task, MessageId id, Message msg);

static bool Ucs146e0ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
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
        DEBUG_LOG("Ucs146e0ReadRegister faild,result = %d\n", result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

/*! \brief Write to a proximity sensor register */
static bool Ucs146e0WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
{
    bitserial_result result;
    uint8 command[2] = {reg, value};

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            command, 2,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        DEBUG_LOG("Ucs146e0WriteRegister faild,result = %d\n", result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

static bitserial_handle Ucs146e0Enable(void)
{
    return hwi2cOpen(UCS146E0_I2C_ADDR, UCS146E0_I2C_FREQ);
}

static void Ucs146e0Disable(bitserial_handle handle)
{
    hwi2cClose(handle);
}

ucs146e0_str ucs146e0_init_array[] = {
    {0x03, 0x1c},
    {0x06, 0x10},
    {0x07, 0x04},
    {0x08, 0x04},
    {0x09, 0x07},
    {0x0a, 0xb1},
    {0x0b, 0x31},//保持三次阈值采样才触发入耳出耳中断
    {0x26, 0x1f},
    {0x10, UCS146E0_LOW_VALUE_L},
    {0x11, UCS146E0_LOW_VALUE_H},
    {0x12, UCS146E0_HIGH_VALUE_L},
    {0x13, UCS146E0_HIGH_VALUE_H},
    {0x2a, 0},//校准low值
    {0x2b, 0},//校准high值
    {0x00, 0x46},
};

ucs146e0_str ucs146e0_get_auto_calvalue_init_array[] = {
    {0x03, 0x1c},
    {0x06, 0x10},
    {0x07, 0x04},
    {0x08, 0x04},
    {0x09, 0x07},
    {0x0a, 0xb1},
    {0x26, 0x0f},
    {0x2a, 0},//校准low值
    {0x2b, 0},//校准high值
    {0x00, 0x46},
};

ucs146e0_str ucs146e0_cal_init_array[] = {
    {0x03, 0x1c},
    {0x06, 0x10},
    {0x07, 0x04},
    {0x08, 0x04},
    {0x09, 0x07},
    {0x0a, 0xb1},
    {0x26, 0x1f},
    {0x2a, 0},//校准low值
    {0x2b, 0},//校准high值
    {0x00, 0x46},
};

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

#define TIME_READ_Ucs146e0_REG
extern FixParam gFixParam;
typedef struct tagSHELLCMDINFO {
    TaskData       task;
    bool status;
}Ucs146e0InfoTask;
static Ucs146e0InfoTask *Ucs146e0Task = NULL;
#ifdef TIME_READ_Ucs146e0_REG
static Ucs146e0InfoTask *time_funcTask = NULL;
#endif

static void Ucs146e0_itr_read_reg(Task task, MessageId id, Message msg)
{
    (void)id;(void)msg;(void)task;
    bitserial_handle handle;
    uint8 value, ear_status;
    proximityTaskData *prox = appGetProximity();
    handle = Ucs146e0Enable();

    Ucs146e0ReadRegister(handle, 2, &value);
    ear_status = value & 0x20;//obj位为1表示入耳中断，为0表示出耳中断
    Ucs146e0WriteRegister(handle, 2, 0);//clear itr

    if(0 == _ucs146e0Runing) {      // poweroff后，不向外发送消息
        DEBUG_LOG("ucs146e0_itr_read_reg but Poweroff");
        Ucs146e0Disable(handle);
        return;
    }

    if(ear_status &&
            (prox->state->proximity != proximity_state_in_proximity) ){
        prox->state->proximity = proximity_state_in_proximity;
        if (NULL != prox->clients){
            DEBUG_LOG("in ear");
            appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_IN_PROXIMITY);
        }
        _ucs146e0StatInOut = 0;
        if(0 == g_commuType && appInitCompleted() )
            MessageSend(appGetUiTask(), APP_PSENSOR_INEAR, NULL);
    }
    if((!ear_status) &&
            (prox->state->proximity == proximity_state_in_proximity) ){
        prox->state->proximity = proximity_state_not_in_proximity;
        if (NULL != prox->clients){
            DEBUG_LOG("out ear");
            appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY);
        }
        _ucs146e0StatInOut = 1;
        if(0 == g_commuType && appInitCompleted() )
            MessageSend(appGetUiTask(), APP_PSENSOR_OUTEAR, NULL);
    }

    Ucs146e0Disable(handle);
}

#ifdef TIME_READ_Ucs146e0_REG
#define MESSAGE_Ucs146e0_TIME_TRIGGER 1
static void ucs146e0_timer_restart(int timeout)
{
    MessageCancelAll(&time_funcTask->task, MESSAGE_Ucs146e0_TIME_TRIGGER);
    MessageSendLater(&time_funcTask->task, MESSAGE_Ucs146e0_TIME_TRIGGER, 0, timeout);
}

static void Ucs146e0_time_itr_handler(Task task, MessageId id, Message msg)
{
    switch (id){
        case MESSAGE_Ucs146e0_TIME_TRIGGER:
            if(!(PioGet32Bank(PIO2BANK(UCS146E0_ITR_PIN)) & PIO2MASK(UCS146E0_ITR_PIN))) {
                Ucs146e0_itr_read_reg(task, id, msg);
                DEBUG_LOG("Ucs146e0 timer READ");
                ucs146e0_timer_restart(100);
            }
        break;
    }
}
#else
static void ucs146e0_timer_restart(int timeout) { (void)timeout;}
#endif

static void Ucs146e0_itr_handler(Task task, MessageId id, Message msg)
{
    (void)task;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = UCS146E0_ITR_PIN;
    pin = PIO2MASK(pin);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                if(2 == g_commuType){
                    char* buff = "check SENSOR SUSS";
                    CommpcMessage((uint8*)buff, 20);
                }
                Ucs146e0_itr_read_reg(task, id, msg);
                ucs146e0_timer_restart(100);
            }
            break;
        default:
            DEBUG_LOG("id=%d\n", id);
            break;
    }
}

int Ucs146e0_statcheck(void)
{
    if(1 == _ucs146e0Runing)
        return _ucs146e0StatInOut;
    else
        return 0xFF;
}

int Ucs146e0_GetStatus(void)
{
    if(Ucs146e0Task && Ucs146e0Task->status == TRUE){
        return 0;
    }else{
        return -1;
    }
}

int Ucs146e0Power(bool isOn)
{
    int ret;
    bitserial_handle handle;
    handle = Ucs146e0Enable();
    if(isOn){
        ret = Ucs146e0WriteRegister(handle, 0x00, 0x46);
        _ucs146e0Runing = 1;
    }else{
        ret = Ucs146e0WriteRegister(handle, 0x00, 0);
        _ucs146e0Runing = 0;
    }
    Ucs146e0Disable(handle);
    DEBUG_LOG("Ucs146e0Power %d", isOn);
    return ret;
}

void Ucs146e0_get_crosstalk_init(void)//260ms后读取crosstalk值
{
    uint8 i;
    bitserial_handle handle;
    handle = Ucs146e0Enable();
    for(i=0; i<ARRAY_DIM(ucs146e0_get_auto_calvalue_init_array); i++){
        Ucs146e0WriteRegister(handle,
                ucs146e0_get_auto_calvalue_init_array[i].reg,
                ucs146e0_get_auto_calvalue_init_array[i].value);
    }
    Ucs146e0Disable(handle);
}

unsigned short Ucs146e0_get_crosstalk_calvalue(void)
{
    uint8 value;
    unsigned short value_16;
    bitserial_handle handle;
    handle = Ucs146e0Enable();

    Ucs146e0ReadRegister(handle, 0x2d, &value);
    value_16 = value;
    value_16 = value_16<<8;
    Ucs146e0ReadRegister(handle, 0x2c, &value);
    value_16 += value;

    Ucs146e0Disable(handle);
    return value_16;
}

void Ucs146e0_get_ps_cal_init(unsigned short cal_value)//初始化完需200ms延时后读取ps值
{
    uint8 i;
    bitserial_handle handle;
    handle = Ucs146e0Enable();
    for(i=0; i<ARRAY_DIM(ucs146e0_cal_init_array); i++){
        if(ucs146e0_cal_init_array[i].reg == 0x2a){
            ucs146e0_cal_init_array[i].value = cal_value & 0xff;
        }
        if(ucs146e0_cal_init_array[i].reg == 0x2b){
            ucs146e0_cal_init_array[i].value = ((cal_value & 0xff00) >> 8);
        }
    }
    for(i=0; i<ARRAY_DIM(ucs146e0_cal_init_array); i++){
        Ucs146e0WriteRegister(handle,
                ucs146e0_cal_init_array[i].reg,
                ucs146e0_cal_init_array[i].value);
    }
    Ucs146e0Disable(handle);
}

unsigned short Ucs146e0_get_ps_calvalue(void)
{
    uint8 value;
    unsigned short value_16;
    bitserial_handle handle;
    handle = Ucs146e0Enable();

    Ucs146e0ReadRegister(handle, 0x1b, &value);
    value_16 = value;
    value_16 = value_16<<8;
    Ucs146e0ReadRegister(handle, 0x1a, &value);
    value_16 += value;

    Ucs146e0Disable(handle);
    return value_16;
}

void Ucs146e0_init(void)
{
    uint16 bank;
    uint32 mask;
    bitserial_handle handle;
    uint8 value;uint8 i;
    value = 0;

    handle = Ucs146e0Enable();
    PanicFalse(handle != BITSERIAL_HANDLE_ERROR);

    bank = PIO2BANK(UCS146E0_ITR_PIN);
    mask = PIO2MASK(UCS146E0_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
    PioSetWakeupStateBank( bank,  mask,  0);
    PioSetDeepSleepEitherLevelBank( bank,  mask,  mask);

    Ucs146e0ReadRegister(handle, 0xbd, &value);
    Ucs146e0ReadRegister(handle, 0xbc, &value);
    if(value == 0x11)
        DEBUG_LOG("Ucs146e0 id = 0x%x\n", value);
    else{
        Ucs146e0Disable(handle);
        DEBUG_LOG("Ucs146e0 read id error!value = %d\n", value);
        return;
    }
    delay_ms(1);

    if(gFixParam.em20168_cal_already == 1){//说明校准过了，使用校准过的value
        for(i=0; i<ARRAY_DIM(ucs146e0_init_array); i++){
            if(ucs146e0_init_array[i].reg == 0x10){
                ucs146e0_init_array[i].value = gFixParam.em20168_low_value & 0xff;
            }
            if(ucs146e0_init_array[i].reg == 0x11){
                ucs146e0_init_array[i].value = (gFixParam.em20168_low_value & 0xff00) >> 8;
            }
            if(ucs146e0_init_array[i].reg == 0x12){
                ucs146e0_init_array[i].value = gFixParam.em20168_high_value & 0xff;
            }
            if(ucs146e0_init_array[i].reg == 0x13){
                ucs146e0_init_array[i].value = (gFixParam.em20168_high_value & 0xff00) >> 8;
            }
            if(ucs146e0_init_array[i].reg == 0x2a){
                ucs146e0_init_array[i].value = gFixParam.ucs146e0_calib & 0xff;
            }
            if(ucs146e0_init_array[i].reg == 0x2b){
                ucs146e0_init_array[i].value = (gFixParam.ucs146e0_calib & 0xff00) >> 8;
            }
        }
        DEBUG_LOG("ucs146e0 already cal, use cal value!");
    }else{
        DEBUG_LOG("ucs146e0 not cal, use default value!");
    }

    for(i=0; i<ARRAY_DIM(ucs146e0_init_array); i++){
        Ucs146e0WriteRegister(handle,
                ucs146e0_init_array[i].reg,
                ucs146e0_init_array[i].value);
        DEBUG_LOG("reg 0x%x = 0x%x\n", ucs146e0_init_array[i].reg, ucs146e0_init_array[i].value);
    }

    Ucs146e0WriteRegister(handle, 2, 0);//clear itr

    Ucs146e0Task = PanicUnlessNew(Ucs146e0InfoTask);
    memset(Ucs146e0Task, 0, sizeof(Ucs146e0Task));
    Ucs146e0Task->task.handler = Ucs146e0_itr_handler;
    InputEventManagerRegisterTask(&Ucs146e0Task->task, UCS146E0_ITR_PIN);
    Ucs146e0Task->status = TRUE;
    _ucs146e0Runing = 1;

#ifdef TIME_READ_Ucs146e0_REG
    time_funcTask = PanicUnlessNew(Ucs146e0InfoTask);
    memset(time_funcTask, 0, sizeof(time_funcTask));
    time_funcTask->task.handler = Ucs146e0_time_itr_handler;
#endif

    Ucs146e0Disable(handle);
}

#if defined(INCLUDE_PROXIMITY) && !defined(HAVE_EM20168)
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
        InputEventManagerUnregisterTask(&prox->task, UCS146E0_ITR_PIN);

        /* Reset into lowest power mode in case the sensor is not powered off. */
        prox->handle = BITSERIAL_HANDLE_ERROR;
    }
}
#endif

#endif
