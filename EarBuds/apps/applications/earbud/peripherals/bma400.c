#include "bma400.h"
#ifdef INCLUDE_ACCELEROMETER
#ifdef HAVE_BMA400

#define PIO2BANK(pio) ((uint16)((pio) / 32))
/*! \brief Returns the PIO bit position mask within a bank.
    \param pio The pio.
*/
#define PIO2MASK(pio) (1UL << ((pio) % 32))

/*! \brief Read a register from the proximity sensor */
bool BMA400ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len)
{
    bitserial_result result;
    /* First write the register address to be read */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            &reg, 1,
                            BITSERIAL_FLAG_BLOCK);
    if (result == BITSERIAL_RESULT_SUCCESS)
    {
        /* Now read the actual data in the register */
        result = BitserialRead(handle,
                                BITSERIAL_NO_MSG,
                                value, len,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        printf("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

/*! \brief Write to a proximity sensor register */
bool BMA400WriteRegister(bitserial_handle handle, uint8 *msg, uint8 len)
{
    bitserial_result result;
    //uint8 command[2] = {reg, value};

    /* Write the write command and register */
    result = BitserialWrite(handle,
                            BITSERIAL_NO_MSG,
                            msg, len,
                            BITSERIAL_FLAG_BLOCK);
    if(result != BITSERIAL_RESULT_SUCCESS){
        printf("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bitserial_handle BMA400Enable(void)
{
    bitserial_config bsconfig;
    uint32 i;
    uint16 bank;
    uint32 mask;
    struct
    {
        uint16 pio;
        pin_function_id func;
    } i2c_pios[] = {{BMA400_I2C_CLK_PIN, BITSERIAL_1_CLOCK_OUT},
                    {BMA400_I2C_CLK_PIN, BITSERIAL_1_CLOCK_IN},
                    {BMA400_I2C_DATA_PIN, BITSERIAL_1_DATA_OUT},
                    {BMA400_I2C_DATA_PIN, BITSERIAL_1_DATA_IN}};

    DEBUG_LOG("BMA400Enable");
    bank = PIO2BANK(BMA400_ITR_PIN);
    mask = PIO2MASK(BMA400_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

    for (i = 0; i < ARRAY_DIM(i2c_pios); i++)
    {
        uint16 pio = i2c_pios[i].pio;
        bank = PIO2BANK(pio);
        mask = PIO2MASK(pio);

        /* Setup I2C PIOs with strong pull-up */
        PanicNotZero(PioSetMapPins32Bank(bank, mask, 0));
        PanicFalse(PioSetFunction(pio, i2c_pios[i].func));
        PanicNotZero(PioSetDir32Bank(bank, mask, 0));
        PanicNotZero(PioSet32Bank(bank, mask, mask));
        PanicNotZero(PioSetStrongBias32Bank(bank, mask, mask));
    }

    /* Configure Bitserial to work with vncl3020 proximity sensor */
    memset(&bsconfig, 0, sizeof(bsconfig));
    bsconfig.mode = BITSERIAL_MODE_I2C_MASTER;
    bsconfig.clock_frequency_khz = BMA400_I2C_FREQ;
    bsconfig.u.i2c_cfg.i2c_address = BMA400_I2C_ADDR;
    return BitserialOpen((bitserial_block_index)BITSERIAL_BLOCK_1, &bsconfig);
}

struct{
    uint8 reg;
    uint8 value;
}read_array[] = {
    {0x19, 0x0},
    {0x1a, 0x0},
    {0x1b, 0x0},
    {0x57, 0x0},
    {0x58, 0x0},
    {0x21, 0x0},
    {0x22, 0x0},
    {0x23, 0x0},
    {0x1f, 0x0},
    {0x20, 0x0},
};

void BMA400_init(bitserial_handle handle)
{
    uint8 value;
    uint8 i;
//    bitserial_handle handle;
    uint8 value_a[3];
    uint8 value_to_w[4];
//    handle = BMA400Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }
    BMA400ReadRegister(handle, 0x00, value_a, 1);
    BMA400ReadRegister(handle, 0x00, value_a, 1);
    printf("BMA400 id = 0x%x\n", value_a[0]);

    BMA400ReadRegister(handle, 0x19, value_a, 3);
    value_a[0] = (value_a[0] & 0x80) | 0x80;//BMA400_ACCEL_FILT1_BW_1
    value_a[1] = (value_a[1] & 0x0f) | 0x09;//BMA400_ODR_200HZ
    value_a[1] = (value_a[1] & 0xc0) | 0x40;//BMA400_4G_RANGE
    value_a[2] = (value_a[2] & 0x0c) | 0x00;//BMA400_DATA_SRC_ACCEL_FILT_1
    value_to_w[0] = 0x19;
    memcpy(value_to_w+1, value_a, 3);
    BMA400WriteRegister(handle, value_to_w, 4);

    BMA400ReadRegister(handle, 0x57, value_a, 2);
    value_a[0] = (value_a[0] & (~(0x3<<3))) | (0x0<<3);//BMA400_Z_AXIS_EN_TAP
    value_a[0] = (value_a[0] & (~(0x7<<0))) | (0x0<<3);//BMA400_TAP_SENSITIVITY_0
    value_to_w[0] = 0x57;
    memcpy(value_to_w+1, value_a, 2);
    BMA400WriteRegister(handle, value_to_w, 3);

    BMA400ReadRegister(handle, 0x23, &value, 1);
    printf("reg 0x%x = 0x%x\n", 0x23, value);

    BMA400ReadRegister(handle, 0x1f, value_a, 2);
    value_a[1] = (value_a[1] & (~(0x3<<2))) | (0x3<<2);//BMA400_EN_D_TAP,BMA400_EN_S_TAP
    value_a[1] = (value_a[1] & (~(0x1<<7))) | (0x1<<7);//BMA400_EN_D_TAP,BMA400_EN_S_TAP
//    value_to_w[0] = 0x1f;
//    memcpy(value_to_w+1, value_a, 2);
//    BMA400WriteRegister(handle, value_to_w, 3);
    value_to_w[0] = 0x20;
    value_to_w[1] = value_a[1];
    BMA400WriteRegister(handle, value_to_w, 2);
    BMA400ReadRegister(handle, 0x1f, &value, 1);
    printf("reg 0x%x = 0x%x\n", 0x1f, value);
    BMA400ReadRegister(handle, 0x20, &value, 1);
    printf("reg 0x%x = 0x%x\n", 0x20, value);

    BMA400ReadRegister(handle, 0x21, value_a, 3);
    value_a[2] = (value_a[2] & (~(0x1<<2))) | (0x1<<2);//BMA400_TAP_MAP_INT1
    value_a[2] = (value_a[2] & (~(0x1<<6))) | (0x0<<6);//BMA400_TAP_MAP_INT2
    value_to_w[0] = 0x21;
    memcpy(value_to_w+1, value_a, 3);
    //BMA400WriteRegister(handle, value_to_w, 4);
    value_to_w[0] = 0x23;
    value_to_w[1] = value_a[2];
    BMA400WriteRegister(handle, value_to_w, 2);

    value_to_w[0] = 0x24;
    value_to_w[1] = 0x44;//int1,2 pin open drain
    BMA400WriteRegister(handle, value_to_w, 2);


    BMA400ReadRegister(handle, 0x19, value_a, 1);
    value_a[0] = (value_a[0] & (~(0x3<<0))) | (0x2<<0);//BMA400_NORMAL_MODE
    value_to_w[0] = 0x19;
    memcpy(value_to_w+1, value_a, 1);
    BMA400WriteRegister(handle, value_to_w, 2);

    for(i=0; i<ARRAY_DIM(read_array); i++){
        BMA400ReadRegister(handle, read_array[i].reg, &value, 1);
        printf("reg 0x%x = 0x%x\n", read_array[i].reg, value);
    }

    return;
}

void BMA400_itr_handler(Task task, MessageId id, Message msg)
{
    uint8 value_a[3];
    printf("task=0x%x, id=0x%x, msg=0x%x\n",task, id, msg);
    accelTaskData *accel = (accelTaskData *) task;
    BMA400ReadRegister(accel->handle, 0x0e, value_a, 3);
    printf("reg0x0e=0x%x, reg0x0f=0x%x, reg0x10=0x%x\n",value_a[0], value_a[1], value_a[2]);
}

bool appAccelerometerClientRegister(Task task)
{
    accelTaskData *accel = appGetAccelerometer();
    if (NULL == accel->clients)
    {
//        uint8 device_id, part_id, revision;
//        adxl362_activity_control_register_t activity_control = {0};
//        adxl362_power_control_register_t power_control = {0};
//        adxl362_interrupt_control_register_t interrupt_control = {0};

        accel->clients = appTaskListInit();

        accel->handle = BMA400Enable();
        PanicFalse(accel->handle != BITSERIAL_HANDLE_ERROR);
        BMA400_init(accel->handle);

        /* Register for interrupt events */
        accel->task.handler = BMA400_itr_handler;
        InputEventManagerRegisterTask(&accel->task, BMA400_ITR_PIN);
    }

    /* Send a state message to the registering client: read the interrupt PIO state */
    //uint16 bank = PIO2BANK(BMA400_ITR_PIN);
    //uint32 mask = PIO2MASK(BMA400_ITR_PIN);
    MessageSend(task, ACCELEROMETER_MESSAGE_NOT_IN_MOTION, NULL);

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
//    accelTaskData *accel = appGetAccelerometer();
//    uint8 interrupt = accel->config->pios.interrupt;

//    if (appConfigPioIsLed(interrupt))
//    {
//        *key = LED_WAKE_MASK;
//        *value = 1 << appConfigPioLedNumber(interrupt);
//    }
//    else if (appConfigPioCanWakeFromDormant(interrupt))
//    {
//        *key = PIO_WAKE_MASK;
//        *value = 1 << interrupt;
//    }
//    else
//    {
//        DEBUG_LOGF("The accelerometer interrupt PIO (%d) cannot wake the chip from dormant", interrupt);
//        return FALSE;
//    }
    (void*)key;
    (void*)value;
    return TRUE;
}

#endif
#endif
