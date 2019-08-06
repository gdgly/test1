#include "em20168.h"
#ifdef INCLUDE_PROXIMITY
#ifdef HAVE_EM20168

#define PIO2BANK(pio) ((uint16)((pio) / 32))
/*! \brief Returns the PIO bit position mask within a bank.
    \param pio The pio.
*/
#define PIO2MASK(pio) (1UL << ((pio) % 32))

/*! \brief Read a register from the proximity sensor */
bool EM20168ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
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
                                value, 1,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        printf("%s faild,result = %d\n",__func__, result);
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
    bitserial_config bsconfig;
    uint32 i;
    uint16 bank;
    uint32 mask;
    struct
    {
        uint16 pio;
        pin_function_id func;
    } i2c_pios[] = {{EM20168_I2C_CLK_PIN, BITSERIAL_1_CLOCK_OUT},
                    {EM20168_I2C_CLK_PIN, BITSERIAL_1_CLOCK_IN},
                    {EM20168_I2C_DATA_PIN, BITSERIAL_1_DATA_OUT},
                    {EM20168_I2C_DATA_PIN, BITSERIAL_1_DATA_IN}};

    DEBUG_LOG("EM20168Enable");
#ifndef EM20168_KEY_ITR_TEST
    bank = PIO2BANK(EM20168_ITR_PIN);
    mask = PIO2MASK(EM20168_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
#else
    bank = PIO2BANK(EM20168_KEY_ITR_PIN);
    mask = PIO2MASK(EM20168_KEY_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
#endif
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
    bsconfig.clock_frequency_khz = EM20168_I2C_FREQ;
    bsconfig.u.i2c_cfg.i2c_address = EM20168_I2C_ADDR;
    return BitserialOpen((bitserial_block_index)BITSERIAL_BLOCK_1, &bsconfig);
}

em20168_str em20168_init_array[] = {
    {0x01, 0x80},
    {0x0b, 0x80},
    {0x0c, 0xc0},
    {0x0d, 0x3c},
    {0x0e, 0x71},
    {0x24, 0x30},
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

void EM20168_init(void)
{
    bitserial_handle handle;
    uint8 value;
    uint8 i;
    value = 0;
    handle = EM20168Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }
    EM20168ReadRegister(handle, 0x00, &value);
    EM20168ReadRegister(handle, 0x00, &value);
    printf("em20168 id = 0x%x\n", value);

    printf("em20168 init num = %d\n", ARRAY_DIM(em20168_init_array));
    for(i=0; i<ARRAY_DIM(em20168_init_array); i++){
        EM20168WriteRegister(handle,
                em20168_init_array[i].reg,
                em20168_init_array[i].value);
    }
    return;
}

void EM20168_itr_handler(Task task, MessageId id, Message msg)
{
    uint8 value;
    unsigned short em20168_ps0_value;
    unsigned long pin, pio_state;
    proximityTaskData *prox = (proximityTaskData *) task;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
#ifndef EM20168_KEY_ITR_TEST
    pin = EM20168_ITR_PIN;
#else
    pin = EM20168_KEY_ITR_PIN;
#endif
    //printf("in ear\n\n");
    pin = PIO2MASK(pin);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;
    EM20168WriteRegister(prox->handle, 2, 0);
    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                EM20168ReadRegister(prox->handle, 0x21, &value);
                em20168_ps0_value = value << 8;
                EM20168ReadRegister(prox->handle, 0x20, &value);
                em20168_ps0_value += value;
                //printf("reg = 0x%x\n\n", em20168_ps0_value);
                if(em20168_ps0_value >= EM20168_HIGH_VALUE &&
                        (prox->state->proximity != proximity_state_in_proximity) ){
                    prox->state->proximity = proximity_state_in_proximity;
#ifndef EM20168_SEND_MSG
                    printf("in ear\n\n", em20168_ps0_value);
#else
                    appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_IN_PROXIMITY);
#endif
                }
                if(em20168_ps0_value <= EM20168_LOW_VALUE &&
                        (prox->state->proximity == proximity_state_in_proximity) ){
                    prox->state->proximity = proximity_state_not_in_proximity;
#ifndef EM20168_SEND_MSG
                    printf("out ear\n\n", em20168_ps0_value);
#else
                    appTaskListMessageSendId(prox->clients, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY);
#endif
                }
            }
            break;
        default:
            printf("id=%d(0x%x\n", id, id);
            break;
    }
}

bool appProximityClientRegister(Task task)
{
    uint8 value;
    uint8 i;
    proximityTaskData *prox = appGetProximity();
    if (NULL == prox->clients)
    {
        prox->state = PanicUnlessNew(proximityState);
        prox->state->proximity = proximity_state_unknown;
        prox->clients = appTaskListInit();

        prox->handle = EM20168Enable();
        PanicFalse(prox->handle != BITSERIAL_HANDLE_ERROR);

        EM20168ReadRegister(prox->handle, 0x00, &value);
        EM20168ReadRegister(prox->handle, 0x00, &value);
        printf("em20168 id = 0x%x\n", value);

        printf("em20168 init num = %d\n", ARRAY_DIM(em20168_init_array));
        for(i=0; i<ARRAY_DIM(em20168_init_array); i++){
            EM20168WriteRegister(prox->handle,
                    em20168_init_array[i].reg,
                    em20168_init_array[i].value);
        }
        for(i=0; i<ARRAY_DIM(em20168_read_array); i++){
            EM20168ReadRegister(prox->handle, em20168_read_array[i].reg, &value);
            printf("reg 0x%x = 0x%x\n", em20168_read_array[i].reg, value);
        }

        /* Register for interrupt events */
        prox->task.handler = EM20168_itr_handler;
#ifndef EM20168_KEY_ITR_TEST
        InputEventManagerRegisterTask(&prox->task, EM20168_ITR_PIN);
#else
        InputEventManagerRegisterTask(&prox->task, EM20168_KEY_ITR_PIN);
#endif
    }
    /* Send initial message to client */
#ifdef EM20168_SEND_MSG
    switch (prox->state->proximity)
    {
        case proximity_state_in_proximity:
            MessageSend(task, PROXIMITY_MESSAGE_IN_PROXIMITY, NULL);
            break;
        case proximity_state_not_in_proximity:
            MessageSend(task, PROXIMITY_MESSAGE_NOT_IN_PROXIMITY, NULL);
            break;
        case proximity_state_unknown:
        default:
            /* The client will be informed after the first interrupt */
            break;
    }
#endif

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
#ifndef EM20168_KEY_ITR_TEST
        InputEventManagerUnregisterTask(&prox->task, EM20168_ITR_PIN);
#else
        InputEventManagerUnregisterTask(&prox->task, EM20168_KEY_ITR_PIN);
#endif
        /* Reset into lowest power mode in case the sensor is not powered off. */
        prox->handle = BITSERIAL_HANDLE_ERROR;
    }
}

#endif
#endif
