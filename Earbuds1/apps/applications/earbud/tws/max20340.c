#include "max20340.h"
#ifdef HAVE_MAX20340

/*! \brief Read a register from the proximity sensor */
bool max20340ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
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

bool max20340ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len)
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
                                value, len,
                                BITSERIAL_FLAG_BLOCK);
    }
    if(result != BITSERIAL_RESULT_SUCCESS){
        //printf("%s faild,result = %d\n",__func__, result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

/*! \brief Write to a proximity sensor register */
bool max20340WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
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

unsigned int used_i2c_addr=0;
bitserial_handle max20340Enable(void)
{
    uint16 bank;
    uint32 mask;

    bank = PIO2BANK(MAX20340_ITR_PIN);
    mask = PIO2MASK(MAX20340_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

    return hwi2cOpen(used_i2c_addr, MAX20340_I2C_FREQ);
}

void max20340Disable(bitserial_handle handle)
{
    //printf("EM20168Disable");
    hwi2cClose(handle);
}

void singlebus_itr_handler(Task task, MessageId id, Message msg)
{
    uint8 value_a[0x12];
    //(void)id;(void)msg;(void)task;
    (void)task;
    bitserial_handle handle;
//    uint8 i;
//    uint8 value;
    unsigned long pin, pio_state;
    MessagePioChanged *pioMsg = (MessagePioChanged*)msg;
    pin = PIO2MASK(MAX20340_ITR_PIN);
    pio_state = pioMsg->state16to31 << 16 | pioMsg->state;

    switch(id) {
        case MESSAGE_PIO_CHANGED:
            if( !(pin&pio_state) ){
                handle = max20340Enable();
                max20340ReadRegister_withlen(handle, 0x0, value_a, 0xa);
                max20340ReadRegister_withlen(handle, 0x10, value_a, 3);
                //printf("max20340 read value = 0x%x, 0x%x, 0x%x\n", value_a[0], value_a[1], value_a[2]);
                max20340WriteRegister(handle, 0xd, 0xff);
            //    for(i=0; i<0x12; i++){
            //        max20340ReadRegister(handle, i, &value);
            //        printf("max20340 reg 0x%x = 0x%x\n", i, value);
            //    }
            //    max20340WriteRegister(handle, 0xd, 0x02);
            //    max20340WriteRegister(handle, 0xe, 0x60);
            //    max20340WriteRegister(handle, 0xf, 0x02);
                max20340Disable(handle);
            }
            break;
        default:
            printf("id=%d(0x%x\n", id, id);
            break;
    }
}

typedef struct tagSHELLCMDINFO {
    TaskData       task;
}singlebus_funcInfoTask;
static singlebus_funcInfoTask *psbfuncTask = NULL;


int max20340_get_left_or_right(void)
{
    bitserial_handle handle;
    uint8 value;
    static uint8 ret_value = 0;
    if( (ret_value!=1) && (ret_value!=2) ){
        value = 0;
        used_i2c_addr = MAX20340_LEFTEAR_I2C_ADDR;
        handle = max20340Enable();
        max20340ReadRegister(handle, 0x00, &value);
        max20340ReadRegister(handle, 0x00, &value);
        max20340Disable(handle);
        if(value == 0x10){
            ret_value = 1;
            return 1;
        }

        value = 0;
        used_i2c_addr = MAX20340_RIGHTEAR_I2C_ADDR;
        handle = max20340Enable();
        max20340ReadRegister(handle, 0x00, &value);
        max20340ReadRegister(handle, 0x00, &value);
        max20340Disable(handle);
        if(value == 0x10){
            ret_value = 2;
            return 2;
        }
        return 0;
    }else{
        return ret_value;
    }
}


void max20340_init(void)
{
    bitserial_handle handle;
    uint8 value;
    uint8 i;
    if( max20340_get_left_or_right()==0 ){
        return;
    }
    value = 0;
    handle = max20340Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }
    max20340ReadRegister(handle, 0x00, &value);
    max20340ReadRegister(handle, 0x00, &value);
    printf("max20340 id = 0x%x\n", value);

    max20340WriteRegister(handle, 0x8, 0x7f);
    max20340WriteRegister(handle, 0xc, 0xff);
    for(i=0; i<0x12; i++){
        max20340ReadRegister(handle, i, &value);
        printf("max20340 reg 0x%x = 0x%x\n", i, value);
    }

//    printf("em20168 init num = %d\n", ARRAY_DIM(em20168_init_array));
//    for(i=0; i<ARRAY_DIM(em20168_init_array); i++){
//        EM20168WriteRegister(handle,
//                em20168_init_array[i].reg,
//                em20168_init_array[i].value);
//    }

    psbfuncTask = PanicUnlessNew(singlebus_funcInfoTask);
    memset(psbfuncTask, 0, sizeof(singlebus_funcInfoTask));
    psbfuncTask->task.handler = singlebus_itr_handler;
    InputEventManagerRegisterTask(&psbfuncTask->task, MAX20340_ITR_PIN);

    max20340Disable(handle);
    return;
}

#endif
