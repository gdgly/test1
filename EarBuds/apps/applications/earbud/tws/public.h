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
//              /* Uart For Debug INIT */
//==============================================================================================
#define UART_RX_PIO   (4)
#define UART_TX_PIO   (3)
#define CONFIG_HW_UART
#define CONFIG_SHELL_UART
typedef signed (*UartRxProc)(const uint8 *ptr, int len);
void uartInit(void);
void UartTxData(const uint8 *data, uint16 length);
void UartSetRecvHandle(UartRxProc func);
void ShellCmdInit(void);


//==============================================================================================
//              /* Lis25BA I2C AUDIO INIT */
//==============================================================================================
#define CONFIG_LIS25BA
#define CONFIG_LIS25BA_TEST         // 测试
void lis25Init(void);
int lis25Power(bool isOn);
void lis25Test(void);
int lis25TestPower(bool isOn);


//==============================================================================================
//              /* I2S AUDIO INIT */
//==============================================================================================
void SourceI2SConfigure(Source source);
void SinkI2SConfigure(Sink sink);
void SinkI2SConfigureI2S(Sink sink);
void SourceSinkI2SMasterClockEnable(void *source_sink, int isSource, int enable);   // 如果有多个source/Sink此处只能调用一次

//==============================================================================================
//              /* em20168 Proximity */
//==============================================================================================
#define HAVE_EM20168
void EM20168_init(void);
int EM20168Power(bool isOn);


//==============================================================================================
//              /* bma400 Tap */
//==============================================================================================
#define HAVE_BMA400
void BMA400_init(void);
int BMA400Power(bool isOn);

#endif // PUBLIC_H
