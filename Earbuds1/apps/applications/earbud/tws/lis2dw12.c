#include "lis2dw12.h"
#include "online_dbg.h"
#ifdef HAVE_LIS2DW12

uint8  lis2dw12Runing = 0;              // poweroff后，设置为 0, 启动运行为1
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
        //DEBUG_LOG("lis2dw12ReadRegister faild,result = %d\n", result);
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
        DEBUG_LOG("lis2dw12WriteRegister faild,result = %d\n", result);
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
        //DEBUG_LOG("lis2dw12ReadRegister_withlen faild,result = %d\n", result);
    }
    return (result == BITSERIAL_RESULT_SUCCESS);
}

bitserial_handle lis2dw12Enable(void)
{
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
#if 0
    {0x25, 3<<4,    0<<4},//Set full scale, fs 0
    {0x20, 0xf,     0},//Configure power mode, mode lp_mode 0
    {0x25, 1<<2,    1<<2},//low_noise 1
    {0x20, 0xf<<4,  7<<4},//odr
    {0x22, 3,       0},//slp_mode 0
    //{0x22, 1<<5,    1<<5},//PP_OD 1
    {0x22, 1<<5,    0<<5},//PP_OD 1
    //{0x22, 1<<4,    1<<4},//interrupt request latched
    {0x22, 1<<4,    0<<4},//interrupt not latched
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
#endif
    {0x20, 0xff,    0x74},
    {0x21, 0xff,    0x04},
    {0x22, 0xff,    0x08},
    {0x23, 0xff,    0x08},
    {0x24, 0xff,    0x00},
    {0x25, 0xff,    0x04},
    {0x2e, 0xff,    0x00},
    {0x30, 0xff,    0x00},
    {0x31, 0xff,    0x00},
    {0x32, 0xff,    0x2e},
    {0x33, 0xff,    0x87},
    {0x34, 0xff,    0x80},
    {0x35, 0xff,    0x00},
    {0x36, 0xff,    0x00},
    {0x3c, 0xff,    0x00},
    {0x3d, 0xff,    0x00},
    {0x3e, 0xff,    0x00},
    {0x3f, 0xff,    0x20},
    {0x17, 1<<6,    1<<6},
};

int lis2dw12_get_xyz_cal(lis2dw12_cal_str *cal_p)
{
//    int8 x_cal,y_cal,z_cal;
    bitserial_handle handle;
    uint8 value_arr[6];
    int16 x,y,z; float xf,yf,zf; float off=0.977;
    int16 half = 32767; int half_g = 2;
    handle = lis2dw12Enable();
    lis2dw12ReadRegister_withlen(handle, 0x28, value_arr, 6);
    x = value_arr[1]<<8; x += value_arr[0]; xf = x; xf = xf/half*half_g;
    y = value_arr[3]<<8; y += value_arr[2]; yf = y; yf = yf/half*half_g;
    z = value_arr[5]<<8; z += value_arr[4]; zf = z; zf = zf/half*half_g;
    DEBUG_LOG("x = %d, y= %d, z=%d\r\n",x,y,z);

    cal_p->x_orig = x; cal_p->y_orig = y; cal_p->z_orig = z;

    xf = xf*1000; xf = xf/off;
    yf = yf*1000; yf = yf/off;
    //zf = (1-zf)*1000; zf = zf/off;//芯片朝上
    zf = (-1-zf)*1000; zf = zf/off;//芯片朝下

    if( (xf > 126 || xf < -126) ||
        (yf > 126 || yf < -126) ||
        (zf > 126 || zf < -126) ){
        DEBUG_LOG("error: lis2dw12 cal value over range!");
        lis2dw12Disable(handle);
        return -1;
    }
    cal_p->x_cal = xf; cal_p->y_cal = yf; cal_p->z_cal = zf;
//    x_cal = xf; y_cal = yf; z_cal = zf;
//    printf("xc = %d, yc= %d, zc=%d\r\n",x_cal, y_cal, z_cal);
    lis2dw12Disable(handle);
    return 0;
}

bool lis2dw12_status = FALSE;
int lis2dw12_GetStatus(void)
{
    if(lis2dw12_status == TRUE){
        return 0;
    }else{
        return -1;
    }
}

int Lis2dw12Power(bool isOn)
{
    int ret;
    bitserial_handle handle;
    handle = lis2dw12Enable();
    if(isOn){
        ret = lis2dw12WriteRegister(handle, 0x20, 0x74);
        lis2dw12Runing = 1;
    }else{
        ret = lis2dw12WriteRegister(handle, 0x20, 0x04);
        lis2dw12Runing = 0;
    }
    lis2dw12Disable(handle);
    return ret;
}

void Lis2dw12_reinit_setsens(void)
{
    bitserial_handle handle;
    handle = lis2dw12Enable();
    lis2dw12WriteRegister(handle, 0x32, 0x25);
    lis2dw12WriteRegister(handle, 0x33, 0x47);
    lis2dw12Disable(handle);
}

extern FixParam gFixParam;
void lis2dw12_init(void)
{
    uint16 bank;
    uint32 mask;
    bitserial_handle handle;
    uint8 value;
    uint8 i;
    value = 0;
    handle = lis2dw12Enable();
    if(BITSERIAL_HANDLE_ERROR == handle) {
        online_dbg_record(ONLINE_DBG_TAP_INIT_FAIL);
        return;
    }

    bank = PIO2BANK(LIS2DW12_ITR_PIN);
    mask = PIO2MASK(LIS2DW12_ITR_PIN);
    PanicNotZero(PioSetMapPins32Bank(bank, mask, mask));
    PanicNotZero(PioSetDir32Bank(bank, mask, 0));
    PanicNotZero(PioSet32Bank(bank, mask, mask));
    PioSetWakeupStateBank( bank,  mask,  0);
    PioSetDeepSleepEitherLevelBank( bank,  mask,  mask);

    lis2dw12ReadRegister(handle, 0xf, &value);
    lis2dw12ReadRegister(handle, 0xf, &value);
    DEBUG_LOG("lis2dw12 id = 0x%x\n", value);
    if(value != 0x44){
        lis2dw12Disable(handle);
        DEBUG_LOG("can not get lis2dw12 id!\n");
        online_dbg_record(ONLINE_DBG_TAP_INIT_FAIL);
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

    if(gFixParam.lisdw12_cal_already == 1){
        lis2dw12WriteRegister(handle, 0x3c, gFixParam.lisdw12_cal_x);
        lis2dw12WriteRegister(handle, 0x3d, gFixParam.lisdw12_cal_y);
        lis2dw12WriteRegister(handle, 0x3e, gFixParam.lisdw12_cal_z);
    }

    for(i=0; i<ARRAY_DIM(lis2dw12_init_array); i++){
        lis2dw12ReadRegister(handle, lis2dw12_init_array[i].reg, &value);
        DEBUG_LOG("lis2dw12 reg 0x%x = 0x%x\n", lis2dw12_init_array[i].reg, value);
    }
    lis2dw12_status = TRUE;
    lis2dw12Runing  = 1;

    lis2dw12Disable(handle);

    online_dbg_record(ONLINE_DBG_TAP_INIT_SUCC);

    return;
}

#endif
