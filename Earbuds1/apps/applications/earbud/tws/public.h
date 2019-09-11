#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdio.h>
#include <logging.h>
#include "tws/adv_manager.h"

//==============================================================================================
// TBR 调试器在DEMO板上使用使用的IO脚列表如下，可能会与我们系统中的冲突，注意不能同时使用
//    上面一排右为1脚
//==============================================================================================
//01 PIO-1 	94 	Reset button, RST_N, S2 pin 3 and 4 and TP13, Transaction Bridge, RST_N, J50 pin 14
//02 PIO-2	186 	Transaction bridge, TBR_TX_D3, J50 pin 11, whenJ49 pin 17-18 is connected
//03 PIO-3 	158	Transaction bridge, TBR_TX_D2, J50 pin 10, whenJ49 pin 15-16 is connected
//04 PIO-4 	170	Transaction bridge, TBR_RX_D1, J50 pin 5, whenJ49 pin 7-8 is connected
//05 PIO-5	166 	Transaction bridge, TBR_TX_D1, J50 pin 9, whenJ49 pin 13-14 is connected
//06 PIO-6 	178 	Transaction bridge, TBR_RX_D0, J50 pin 6, whenJ49 pin 9-10 is connected
//07 PIO-7	154 	Transaction bridge, TBR_TX_D0, J50 pin 8, whenJ49 pin 11-12 is connected
//08 PIO-8 	174	Transaction bridge, TBR_RX_CLK, J50 pin 1, whenJ49 pin 1-2 is connected
//09 PIO-15  	123	Battery thermistor, BAT_THERM_VDD, J57 pin 1,using jumper J46 pin 21-22 jumper fitted By default
//10 PIO-16 	73	I2S connector, I2S_CLK, J41 pin 1
//11 PIO-17 	77	I2S connector, I2S_SYNC, J41 pin 2
//12 PIO-18 	81	I2S connector, I2S_0_DOUT, J41 pin 3
//13 PIO-19 	85	I2S connector, I2S_0_DIN, J41 pin 4
//14 PIO-20 	57 ,	J40, SCL, J40 pin 1 and R26■ Display, SCL, U7 pin7 using J39 pin 3-4 jumpe fitted by default
//15 PIO-21	59	J40, SDA, J40 pin 3 and R28■Display, SDA, U7 pin6, using J39 pin 5-6 jumpe fitted by default
//==============================================================================================
// TBR 调试器在我们自己板上使用使用的IO脚列表如下
// 与HAVE_EM20168、HAVE_BMA400有冲突，因此不能同时使用
//==============================================================================================
//15 1V8
//06 PIO-6
//08 PIO-7
//01 PIO-8
//02 GND
//==============================================================================================


/*! \brief Returns the PIOs bank number.
    \param pio The pio.
*/
#define PIO2BANK(pio) ((uint16)((pio) / 32))
/*! \brief Returns the PIO bit position mask within a bank.
    \param pio The pio.
*/
#define PIO2MASK(pio) (1UL << ((pio) % 32))

//==============================================================================================
//              /* PSRAM TEST */
//==============================================================================================
#define CONFIG_PSRAM_TEST            // PsRam IO TEST
#define PSRAM_POWER_PIO    (2)

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
//#define HAVE_EM20168
void EM20168_init(void);
int EM20168Power(bool isOn);


//==============================================================================================
//              /* bma400 Tap */
//==============================================================================================
//#define HAVE_BMA400
void BMA400_init(void);
int BMA400Power(bool isOn);


//==============================================================================================
//            系统功能定义
//==============================================================================================
#define GAIA_TEST                           // 电话数据传输到APP
#define CONFIG_STAROT_PEERPAIR              // 左右耳相互保存BT地址用于配对
#define CONFIG_STAROT_SINGLE                // 独立使用

// BLE 功能使用
#undef  appConfigBleAllowedOutOfCase()
#define appConfigBleAllowedOutOfCase()          (TRUE)
#undef  appConfigBleNewConnectionsWhenBusy()
#define appConfigBleNewConnectionsWhenBusy()    (TRUE)
#undef  appConfigBleUseResolvablePrivateAddress()
#define appConfigBleUseResolvablePrivateAddress() (FALSE)
#undef  appConfigBleGattMtuMin()
#define appConfigBleGattMtuMin()                (88)     // 加上协议头每次可传输80个字节

//==============================================================================================
//            Flash参数读取与保存
//==============================================================================================
bool ParamUsingSingle(void);     // 独立使用
int16 ParamSavePeerAddr( typed_bdaddr *taddr);
int16 ParamLoadPeerAddr( typed_bdaddr *taddr);
int16 ParamLoadBlePair(BlePairInfo *blePairInfo);
int16 ParamSaveBlePair(BlePairInfo *blePairInfo);

//==============================================================================================
//         CAPABILITY 使用定义
//==============================================================================================
#define DOWNLOAD_PASSSTHROUGH                // PASSThrough 取数据
#define DOWNLOAD_G722_ENCODER                // G722压缩将取数据

#endif // PUBLIC_H
