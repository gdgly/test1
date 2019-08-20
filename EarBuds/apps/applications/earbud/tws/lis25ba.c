#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "pio_common.h"

#include "public.h"

#ifdef CONFIG_LIS25BA

/* 器件地址 */
#define LIS25BA_CHIPADDR              (0x19)
#define LIS25BA_I2C_FREQ              (400)           // speed
#define LIS25BA_DEVICE_ID             (0x20)


#define LIS25_REG_WHO_AM_I            (0x0f)
#define LIS25_REG_TDM_cmax_h          (0x24)
#define LIS25_REG_TDM_cmax_l          (0x25)
#define LIS25_REG_CTRL_REG            (0x26)
#define LIS25_REG_TDM_CTRL_REG        (0x2e)
#define LIS25_REG_AXES_CTRL_REG       (0x2f)


typedef struct tagLIS25INFO {
    bitserial_handle handle;
    uint8            inited;
}Lis25Info, *Lis25IPtr;

Lis25Info gLis25Info = {0, 0};

struct{
    uint8    reg, value;
}lis25_regs[] = {{LIS25_REG_CTRL_REG,        0},
                {LIS25_REG_TDM_CTRL_REG,  0x42},
                {LIS25_REG_AXES_CTRL_REG, 0xe1},
                };

static int lis25WriteReg(bitserial_handle handle, uint8 reg, uint8 value)
{
    uint8 cmdbuf[4] = {reg, value, 0, 0};

    if(BITSERIAL_HANDLE_ERROR == handle) {
        handle = hwi2cOpen(LIS25BA_CHIPADDR, LIS25BA_I2C_FREQ);
        if(BITSERIAL_HANDLE_ERROR == handle) {
            return -1;
        }
        hwi2cWrite(handle, cmdbuf, 2);
        hwi2cClose(handle);
    }
    else {
        hwi2cWrite(handle, cmdbuf, 2);
    }

    return 0;
}

void lis25Init(void)
{
    uint8 i, reg, value;
    bitserial_handle handle = BITSERIAL_HANDLE_ERROR;

    handle = hwi2cOpen(LIS25BA_CHIPADDR, LIS25BA_I2C_FREQ);
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }

    reg = LIS25_REG_WHO_AM_I;
    hwi2cRead(handle, &reg, 1,  &value, 1);
    DEBUG_LOG("WhoAmI=0x%x\n", value);
    if(value != LIS25BA_DEVICE_ID) {
        hwi2cClose(handle);
        DEBUG_LOG("Lis25 Read WHOAMI Error=0x%x\n", value);
        return;
    }

    for(i = 0; i < ARRAY_DIM(lis25_regs); i++)
        hwi2cWrite(handle, &lis25_regs[i].reg, 2);

     hwi2cClose(handle);

     gLis25Info.inited = 1;

    return;
}

int lis25Power(bool isOn)
{
    return lis25WriteReg(BITSERIAL_HANDLE_ERROR, LIS25_REG_CTRL_REG, isOn ?  0 : 0x20 );
}

#else
void lis25Init(void) { return; }
int lis25Power(bool isOn) { UNUSED(isOn); return 0; }
#endif








