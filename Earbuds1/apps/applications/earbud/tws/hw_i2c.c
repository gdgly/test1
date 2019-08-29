#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "pio_common.h"
#include "bitserial_api.h"

#include "public.h"

typedef struct tagI2CINFO {
    bitserial_handle handle;
    uint8            inited;
}I2CInfo, *I2CIPtr;

I2CInfo gI2cInfo = {0, 0};

// I2C IO 模式配置
static bool hwi2cInit(void)
{
    uint16 bank, i;
    uint32 mask;

    struct
    {
        uint16 pio;
        pin_function_id func;
    }i2c_pios[] = {{I2C_CLOCK_PIO, BITSERIAL_1_CLOCK_OUT},
                    {I2C_CLOCK_PIO, BITSERIAL_1_CLOCK_IN},
                    {I2C_DATA_PIO, BITSERIAL_1_DATA_OUT},
                    {I2C_DATA_PIO, BITSERIAL_1_DATA_IN}};

    for (i = 0; i < ARRAY_DIM(i2c_pios); i++){
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

    return TRUE;
}

// 打开操作，指定打开的设置地址
bitserial_handle hwi2cOpen(unsigned chipID, unsigned freq)
{
    bitserial_config bsconfig;

    if(!gI2cInfo.inited) {
        hwi2cInit();
        gI2cInfo.inited = 1;
    }

    /* Configure Bitserial to work with vncl3020 proximity sensor */
    memset(&bsconfig, 0, sizeof(bsconfig));
    bsconfig.mode                  = BITSERIAL_MODE_I2C_MASTER;
    bsconfig.clock_frequency_khz   = freq;
    bsconfig.u.i2c_cfg.i2c_address = chipID;

    return BitserialOpen((bitserial_block_index)BITSERIAL_BLOCK_1, &bsconfig);
}

// 打开操作，指定打开的设置地址
bool hwi2cClose(bitserial_handle handle)
{
    BitserialClose(handle);
    return TRUE;
}

// 写操作，按REG + VALUE的形式排列
int8 hwi2cWrite(bitserial_handle handle, uint8 *buffer, uint16 length)
{
    bitserial_result result;

    result = BitserialWrite(handle, BITSERIAL_NO_MSG, buffer, length, BITSERIAL_FLAG_BLOCK);

    return (BITSERIAL_RESULT_SUCCESS == result) ? 0 : result;
}


int8 hwi2cRead(bitserial_handle handle, uint8 *addr, uint16 addrlen, uint8 *buffer, uint16 length)
{
    bitserial_result result;

    result = BitserialTransfer(handle, BITSERIAL_NO_MSG, addr, addrlen, buffer, length);

    return (BITSERIAL_RESULT_SUCCESS == result) ? 0 : result;
}








