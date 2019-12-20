#include "bma400.h"
#ifdef HAVE_BMA400

bma400_str bma400_read_array[] = {
    {0x19, 0x0, 0x0},
    {0x1a, 0x0, 0x0},
    {0x1b, 0x0, 0x0},
    {0x57, 0x0, 0x0},
    {0x58, 0x0, 0x0},
    {0x20, 0x0, 0x0},
    {0x23, 0x0, 0x0},
    {0x24, 0x0, 0x0},
};

bma400_str bma400_init_array[] = {
    {0x24, 0x06, 0x04},//int1 pin open drain
    {0x24, 0x60, 0x40},//int2 pin open drain

    {0x19, 0x80, 0x80},//BMA400_ACCEL_FILT1_BW_1
    {0x1a, 0x0f, 0x09},//BMA400_ODR_200HZ
    {0x1a, 0xc0, 0x40},//BMA400_4G_RANGE
    {0x1b, 0x0c, 0x00},//BMA400_DATA_SRC_ACCEL_FILT_1

    {0x57, 0x18, 0x00},//BMA400_Z_AXIS_EN_TAP
    {0x57, 0x07, 0x00},//BMA400_TAP_SENSITIVITY_0

    #ifdef MUTI_TAP
        {0x20, 0x04, 0x04},//BMA400_EN_S_TAP
    #else
        #ifdef ONLY_DOUBLE_TAP
            {0x20, 0x08, 0x08},//BMA400_EN_D_TAP
        #else
            {0x20, 0x0c, 0x0c},//BMA400_EN_D_TAP,BMA400_EN_S_TAP
        #endif
    #endif
    {0x20, 0x80, 0x80},//latch_int

    {0x23, 0x04, 0x04},//BMA400_TAP_MAP_INT1
    {0x23, 0x40, 0x00},//BMA400_TAP_MAP_INT2 not

    {0x19, 0x03, 0x02},//BMA400_NORMAL_MODE
};

bool BMA400ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
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
        //DEBUG_LOG("BMA400ReadRegisterfaild,result = %d\n", result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bool BMA400WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
{
    bitserial_result result;
    uint8 command[2] = {reg, value};

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            command, 2,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        DEBUG_LOG("BMA400WriteRegister faild,result = %d\n", result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}


bitserial_handle BMA400Enable(void)
{
    uint16 bank;
    uint32 mask;

    //DEBUG_LOG("BMA400Enable");
    bank = PIO2BANK(BMA400_ITR_PIN);
    mask = PIO2MASK(BMA400_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

    return hwi2cOpen(BMA400_I2C_ADDR, BMA400_I2C_FREQ);
}

void BMA400Disable(bitserial_handle handle)
{
    //DEBUG_LOG("BMA400Disable");
    hwi2cClose(handle);
}

void BMA400_init(void)
{
    uint8 value, i;
    bitserial_handle handle;
    handle = BMA400Enable();
    for(i=0; i<ARRAY_DIM(bma400_init_array); i++){
        BMA400ReadRegister(handle,
                bma400_init_array[i].reg,
                &value);
        value &= ~bma400_init_array[i].mask;
        value |= bma400_init_array[i].value;
        BMA400WriteRegister(handle,
                bma400_init_array[i].reg,
                value);
    }
    BMA400Disable(handle);
    return;
}

int BMA400Power(bool isOn)
{
    int ret;
    uint8 value;
    bitserial_handle handle;
    handle = BMA400Enable();
    if(isOn){
        BMA400ReadRegister(handle, 0x19, &value);
        value &= ~0x03;
        value |= 0x02;
        ret = BMA400WriteRegister(handle, 0x19, value);
    }else{
        BMA400ReadRegister(handle, 0x19, &value);
        value &= ~0x03;
        ret = BMA400WriteRegister(handle, 0x19, value);
    }
    BMA400Disable(handle);
    return ret;
}

void BMA400_itr_handler(Task task, MessageId id, Message msg)
{
    uint8 value_a[3];(void)id;(void)msg;
    accelTaskData *accel = (accelTaskData *) task;
    accel->handle = BMA400Enable();
    BMA400ReadRegister(accel->handle, 0x0e, value_a+0);
    BMA400ReadRegister(accel->handle, 0x0f, value_a+1);
    BMA400ReadRegister(accel->handle, 0x10, value_a+2);
    //DEBUG_LOG("reg0x0e=0x%x, reg0x0f=0x%x, reg0x10=0x%x\n",value_a[0], value_a[1], value_a[2]);
    if(value_a[1] & 0x4){
        DEBUG_LOG("single tap\n");
    }
    if(value_a[1] & 0x8){
        DEBUG_LOG("double tap\n");
    }
    BMA400Disable(accel->handle);
}
#ifdef INCLUDE_ACCELEROMETER
bool appAccelerometerClientRegister(Task task)
{
    uint8 value, i;
    accelTaskData *accel = appGetAccelerometer();
    if (NULL == accel->clients)
    {
        accel->clients = appTaskListInit();
        accel->handle = BMA400Enable();
        PanicFalse(accel->handle != BITSERIAL_HANDLE_ERROR);
        for(i=0; i<ARRAY_DIM(bma400_init_array); i++){
            BMA400ReadRegister(accel->handle,
                    bma400_init_array[i].reg,
                    &value);
            value &= ~bma400_init_array[i].mask;
            value |= bma400_init_array[i].value;
            BMA400WriteRegister(accel->handle,
                    bma400_init_array[i].reg,
                    value);
        }
        for(i=0; i<ARRAY_DIM(bma400_read_array); i++){
            BMA400ReadRegister(accel->handle, bma400_read_array[i].reg, &value);
            DEBUG_LOG("bma400 reg 0x%x = 0x%x\n", bma400_read_array[i].reg, value);
        }
        BMA400Disable(accel->handle);
        /* Register for interrupt events */
        accel->task.handler = BMA400_itr_handler;
        InputEventManagerRegisterTask(&accel->task, BMA400_ITR_PIN);
    }

    /* Send a state message to the registering client: read the interrupt PIO state */
    //uint16 bank = PIO2BANK(BMA400_ITR_PIN);
    //uint32 mask = PIO2MASK(BMA400_ITR_PIN);
    //MessageSend(task, ACCELEROMETER_MESSAGE_NOT_IN_MOTION, NULL);

    return appTaskListAddTask(accel->clients, task);
}

void appAccelerometerClientUnregister(Task task)
{
    accelTaskData *accel = appGetAccelerometer();

    appTaskListRemoveTask(accel->clients, task);
    if (0 == appTaskListSize(accel->clients))
    {
        appTaskListDestroy(accel->clients);
        accel->clients = NULL;

        PanicFalse(accel->handle != BITSERIAL_HANDLE_ERROR);

        /* Unregister for interrupt events */
        InputEventManagerUnregisterTask(&accel->task, BMA400_ITR_PIN);

        accel->handle = BITSERIAL_HANDLE_ERROR;
    }
}

bool appAccelerometerGetDormantConfigureKeyValue(dormant_config_key *key, uint32* value)
{
    (void*)key;
    (void*)value;
    return TRUE;
}
#endif

#endif

