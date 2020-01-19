#ifndef PUBLIC_H
#define PUBLIC_H

#include <stdio.h>
#include <logging.h>
#include "tws/adv_manager.h"

//#define CONFIG_BOARD_V1        // V1版本
//#define CONFIG_BOARD_V1_EARBARD //v1耳机板与demo板io不一样，打开此宏表示是耳机

#define CONFIG_BOARD_V2

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
//#define CONFIG_HW_UART
//#define CONFIG_SHELL_UART
typedef signed (*UartRxProc)(const uint8 *ptr, int len);
void uartInit(void);
void UartTxData(const uint8 *data, uint16 length);
void UartSetRecvHandle(UartRxProc func);
void ShellCmdInit(void);
int appChangeCVCProcessMode(void);

//==============================================================================================
//              /* Lis25BA I2C AUDIO INIT */
//==============================================================================================
#define CONFIG_LIS25BA
#define CONFIG_LIS25BA_TEST         // 测试
void lis25Init(void);
int lis25Power(bool isOn);
int lis25GetStatus(void);             //  0is OK.
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
#define EM20168_ITR_PIN 61
void EM20168_init(void);
int EM20168Power(bool isOn);


//==============================================================================================
//              /* bma400 Tap */
//==============================================================================================
//#define HAVE_BMA400
#define BMA400_ITR_PIN 7
void BMA400_init(void);
int BMA400Power(bool isOn);


//==============================================================================================
//              /*Tap 功能 bma400 或者 lis2dw12两种芯片 */
//==============================================================================================
void tap_itr_handle_lis2dw12(void);
void tap_itr_handle_bma400(void);
void tap_itr_handler(Task task, MessageId id, Message msg);
void tap_func_init(void);

//==============================================================================================
//              /* PC Write Test */
//==============================================================================================
#define CUMMPC_PC_USB
void CummuInit(void);
Task GetCommuTask(void);
void CommpcMessage(uint8* buff ,uint8 size);
extern uint16 g_commuType;

//==============================================================================================
//      BOARD_V1 不同定义 硬件上的差别
//==============================================================================================
#ifdef CONFIG_BOARD_V1

#define CONFIG_I2CADDR_FOR_LEFT_RIGHT       // V1版本 使用I2C地址来确认是左右耳
#undef  appConfigLeftAudioChannel
#define appConfigLeftAudioChannel()              (AUDIO_CHANNEL_B)       // 耳机使用右声道输出

/*接近光*/
#define HAVE_EM20168
#undef EM20168_ITR_PIN
#define EM20168_ITR_PIN 8
int EM20168_GetStatus(void);//0 表示初始化ok, -1 wrong
void EM20168_Set_psvalue(bool high_or_low, unsigned short reg_value);//设置产生中断值，high_or_low表示设最大值或最小值
unsigned short EM20168_Get_psvalue(void);//返回当前ps值

/* max20340 single bus */
#define HAVE_MAX20340
#define MAX20340_ITR_PIN 3
#define MAX20340_EN_PIN 4
int max20340_get_left_or_right(void);//返回0没找到，1左耳，2右耳
void max20340_init(void);
int max20340_GetStatus(void);//0 表示初始化ok, -1 wrong

#if (defined(CONFIG_HW_UART) && defined(HAVE_MAX20340))
#error "UART and MAX20340 conflict in PIO3"
#endif

/*tap*/
#define HAVE_LIS2DW12
#define LIS2DW12_ITR_PIN 5
void lis2dw12_init(void);
int lis2dw12_GetStatus(void);//0 表示初始化ok, -1 wrong
int Lis2dw12Power(bool isOn);//1 打开，0关闭

/*耳机软板io重定义*/
#ifdef CONFIG_BOARD_V1_EARBARD
#undef PSRAM_POWER_PIO
#undef MAX20340_ITR_PIN
#define PSRAM_POWER_PIO     3
#define MAX20340_ITR_PIN    2
#endif

#endif

#ifdef CONFIG_BOARD_V2
#undef CONFIG_LIS25BA
#undef UART_RX_PIO
#undef UART_TX_PIO
#define UART_TX_PIO   (53)

#define CONFIG_I2CADDR_FOR_LEFT_RIGHT       // V1版本 使用I2C地址来确认是左右耳
#undef  appConfigLeftAudioChannel
#define appConfigLeftAudioChannel()              (AUDIO_CHANNEL_B)       // 耳机使用右声道输出

/*接近光*/
#define HAVE_EM20168
#undef EM20168_ITR_PIN
#define EM20168_ITR_PIN 8
int EM20168_GetStatus(void);//0 表示初始化ok, -1 wrong
int EM20168_statcheck(void);
void EM20168_Set_psvalue(bool high_or_low, unsigned short reg_value);//设置产生中断值，high_or_low表示设最大值或最小值
unsigned short EM20168_Get_psvalue(void);//返回当前ps值

/* max20340 single bus */
#define HAVE_MAX20340
#define MAX20340_ITR_PIN 2
int max20340_get_left_or_right(void);//返回0没找到，1左耳，2右耳
void max20340_init(void);
int max20340_GetStatus(void);//0 表示初始化ok, -1 wrong

/*tap*/
#define HAVE_LIS2DW12
#define LIS2DW12_ITR_PIN 5
void lis2dw12_init(void);
int lis2dw12_GetStatus(void);//0 表示初始化ok, -1 wrong
int Lis2dw12Power(bool isOn);//1 打开，0关闭

#undef PSRAM_POWER_PIO
#define PSRAM_POWER_PIO     3

#endif

//==============================================================================================
//            系统功能定义
//==============================================================================================
#define GAIA_TEST                           // 电话数据传输到APP
/// todo hjs 调试的时候，走原来的代码，配对方面

#ifndef TWS_DEBUG
#define CONFIG_STAROT_PEERPAIR              // 左右耳相互保存BT地址用于配对
#define CONFIG_STAROT_SINGLE                // 独立使用
#endif

// 打电话时的双MIC使用
#define CONFIG_MIC_SELECT          // 动态选择使用MIC1 或 MIC2 或两个都使用
extern uint8 g_appConfigSocMic1, g_appConfigSocMic2;
#undef appConfigScoMic1()
#define appConfigScoMic1()                    (g_appConfigSocMic1)
#undef appConfigScoMic2()
#define appConfigScoMic2()                    (g_appConfigSocMic2)



// 充电电流及电压使用
#undef  appConfigChargerFastCurrent()
#define appConfigChargerFastCurrent()         (50)
#undef  appConfigChargerTerminationVoltage()
#define appConfigChargerTerminationVoltage()  (4400)
#undef  appConfigBatteryFullyCharged()
#define appConfigBatteryFullyCharged()        (4400)
#undef  appConfigChargerTerminationCurrent()
#define appConfigChargerTerminationCurrent()  (3)

// BLE 功能使用
#undef  appConfigBleAllowedOutOfCase()
#define appConfigBleAllowedOutOfCase()          (TRUE)
#undef  appConfigBleNewConnectionsWhenBusy()
#define appConfigBleNewConnectionsWhenBusy()    (TRUE)
#undef  appConfigBleUseResolvablePrivateAddress()
#define appConfigBleUseResolvablePrivateAddress() (FALSE)
#undef  appConfigBleGattMtuMin()
#define appConfigBleGattMtuMin()                (168)     // 加上协议头每次可传输80个字节

// ================================================
//              audio forward 开关
// ================================================

#define FORWARD_AUDIO_NULL (0)
#define FORWARD_AUDIO_SCO  (1<<0)
#define FORWARD_AUDIO_MIC  (1<<1)
#define FORWARD_AUDIO_TYPE (FORWARD_AUDIO_SCO | FORWARD_AUDIO_MIC)

// ================================================
//              software watchdog 开关
// ================================================

#define ENABLE_WATCHDOG

// ================================================
//              apollo 开关
// ================================================

#define ENABLE_APOLLO

// ================================================
//              录音助手功能
// ================================================
#define CONFIG_REC_ASSISTANT                // 录音助手功能

//==============================================================================================
//            Flash参数读取与保存
//==============================================================================================
#include "param.h"

//==============================================================================================
//         CAPABILITY 使用定义
//==============================================================================================
// #define DOWNLOAD_PASSSTHROUGH                // PASSThrough 取数据
#define DOWNLOAD_G722_ENCODER                // G722压缩将取数据

#endif // PUBLIC_H
