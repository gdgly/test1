#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "pio_common.h"

#include "public.h"

#ifdef CONFIG_LIS25BA

/* 器件地址 */
#define LIS25BA_CHIPID                (0x19)
#define LIS25BA_I2C_FREQ              (400)


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

void lis25Init(void)
{
    uint8 i, reg, value;
    bitserial_handle handle;

    handle = hwi2cOpen(LIS25BA_CHIPID, LIS25BA_I2C_FREQ);
    if(BITSERIAL_HANDLE_ERROR == handle) {
        return;
    }

    reg = LIS25_REG_WHO_AM_I;
    hwi2cRead(handle, &reg, 1,  &value, 1);
    printf("WhoAmI=0x%x\n", value);
    if(value != 0x20) {
        hwi2cClose(handle);
        printf("Lis25 Read WHOAMI Error=0x%x\n", value);
        return;
    }

    for(i = 0; i < ARRAY_DIM(lis25_regs); i++)
        hwi2cWrite(handle, &lis25_regs[i].reg, 2);

     hwi2cClose(handle);

     gLis25Info.inited = 1;

    return;
}

#else
void lis25Init(void) { return; }
#endif








