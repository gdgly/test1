#include "lis2dw12.h"
#ifdef HAVE_LIS2DW12

/*! \brief Read a register from the proximity sensor */
bool lis2dw12ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value)
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
bool lis2dw12WriteRegister(bitserial_handle handle, uint8 reg, uint8 value)
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

bool lis2dw12ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len)
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

bitserial_handle lis2dw12Enable(void)
{
    uint16 bank;
    uint32 mask;

    bank = PIO2BANK(LIS2DW12_ITR_PIN);
    mask = PIO2MASK(LIS2DW12_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));

    return hwi2cOpen(LIS2DW12_I2C_ADDR, LIS2DW12_I2C_FREQ);
}

void lis2dw12Disable(bitserial_handle handle)
{
    hwi2cClose(handle);
}

typedef struct{
    uint8 reg;
    uint8 mask;
    uint8 value;
}lis2dw12_str;
lis2dw12_str lis2dw12_init_array[] = {
    {0x25, 3<<4,    0<<4},//Set full scale, fs 0
    {0x20, 0xf,     0},//Configure power mode, mode lp_mode 0
    {0x25, 1<<2,    1<<2},//low_noise 1
    {0x20, 0xf<<4,  7<<4},//odr
    {0x22, 3,       0},//slp_mode 0
    {0x22, 1<<5,    1<<5},//PP_OD 1
    {0x22, 1<<4,    1<<4},//interrupt request latched
    {0x22, 1<<3,    1<<3},//H_LACTIVE 1 //1: active low
    {0x32, 1<<5,    1<<5},//tap_z_en 1
    {0x32, 1<<6,    1<<6},//tap_y_en 1
    {0x32, 1<<7,    1<<7},//tap_x_en 1
    {0x30, 0x1f,    9},//tap_thsx 12
    {0x31, 0x1f,    9},//tap_thsy 12
    {0x32, 0x1f,    9},//tap_thsz 12
    {0x33, 0xf<<4,  1<<4},//latency 7
    {0x33, 3<<2,    3<<2},//quiet 3
    {0x33, 3,       3},//shock 3
    {0x34, 1<<7,    1<<7},//single_double_tap 1
    {0x23, 1<<3,    1<<3},//int1_tap 1
    {0x3f, 1<<5,    1<<5},//interrupts_enable 1
};

void lis2dw12_init(void)
{
    bitserial_handle handle;
    uint8 value;
    uint8 i;
    value = 0;
    handle = lis2dw12Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }
    lis2dw12ReadRegister(handle, 0xf, &value);
    lis2dw12ReadRegister(handle, 0xf, &value);
    printf("lis2dw12 id = 0x%x\n", value);
    if(value != 0x44){
        lis2dw12Disable(handle);
        printf("can not get lis2dw12 id!\n");
        return;
    }

    for(i=0; i<ARRAY_DIM(lis2dw12_init_array); i++){
        lis2dw12ReadRegister(handle,
                lis2dw12_init_array[i].reg,
                &value);
        value &= ~lis2dw12_init_array[i].mask;
        value |= lis2dw12_init_array[i].value;
        lis2dw12WriteRegister(handle,
                lis2dw12_init_array[i].reg,
                value);
    }
    for(i=0; i<ARRAY_DIM(lis2dw12_init_array); i++){
        lis2dw12ReadRegister(handle, lis2dw12_init_array[i].reg, &value);
        printf("lis2dw12 reg 0x%x = 0x%x\n", lis2dw12_init_array[i].reg, value);
    }

    lis2dw12Disable(handle);
    return;
}

#endif
