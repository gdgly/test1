#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdio.h>

/*! \brief Returns the PIOs bank number.
    \param pio The pio.
*/
#define PIO2BANK(pio) ((uint16)((pio) / 32))
/*! \brief Returns the PIO bit position mask within a bank.
    \param pio The pio.
*/
#define PIO2MASK(pio) (1UL << ((pio) % 32))

//==============================================================================================
//              /* I2C GPIO Select */
//==============================================================================================
#define I2C_CLOCK_PIO      (21)
#define I2C_DATA_PIO       (20)

// 打开操作，指定打开的设置地址
bitserial_handle hwi2cOpen(unsigned chipID, unsigned freq);

// 打开操作，指定打开的设置地址
bool hwi2cClose(bitserial_handle handle);

// 写操作，按REG + VALUE的形式排列
int8 hwi2cWrite(bitserial_handle handle, uint8 *buffer, uint16 length);

// 读取操作，按REG 返回
int8 hwi2cRead(bitserial_handle handle, uint8 *addr, uint16 addrlen, uint8 *buffer, uint16 length);


//==============================================================================================
//              /* Lis25BA I2C AUDIO INIT */
//==============================================================================================
#define CONFIG_LIS25BA
void lis25Init(void);



#endif // PUBLIC_H
