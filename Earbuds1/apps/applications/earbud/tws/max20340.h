#ifndef MAX20340_H
#define MAX20340_H

#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <hydra_macros.h>
#include <input_event_manager.h>
#include "../av_headset.h"
#include "../av_headset_log.h"
#include "../av_headset_accelerometer.h"

#define MX20340_REG_DEVID               (uint8_t)0x00 // CHIP_ID[7:4], CHIP_REV[3:0]
#define MX20340_REG_CTRL1               (uint8_t)0x01 // DET_RST[1], EN[0]
#define MX20340_REG_CTRL2               (uint8_t)0x02 // V_LDO_MIN[7:5], D_LDO_BAT[4:2], TWAIT_TMR[1:0]
#define MX20340_REG_CTRL3               (uint8_t)0x03 // COM_THRS[7:6], CHG_TMR_SET[5:4], BAT_RECHG[2:0]
#define MX20340_REG_CTRL4               (uint8_t)0x04 // TXRX_RESET[4], SLAVE_TO_IDLE[3], TXFILT_ENB[0]
#define MX20340_REG_STA1                (uint8_t)0x05 // Device status register 1
#define MX20340_REG_STA2                (uint8_t)0x06 // Device status register 2
#define MX20340_REG_STA_IRQ             (uint8_t)0x07 // status IRQ
#define MX20340_REG_STA_MASK            (uint8_t)0x08 // status mask
#define MX20340_REG_PLC_CTL             (uint8_t)0x09 //
#define MX20340_REG_PLC_STA             (uint8_t)0x0A //
#define MX20340_REG_PLC_IRQ             (uint8_t)0x0B //
#define MX20340_REG_PLC_MASK            (uint8_t)0x0C //
#define MX20340_REG_TX_DATA0            (uint8_t)0x0D //
#define MX20340_REG_TX_DATA1            (uint8_t)0x0E //
#define MX20340_REG_TX_DATA2            (uint8_t)0x0F //
#define MX20340_REG_RX_DATA0            (uint8_t)0x10 //
#define MX20340_REG_RX_DATA1            (uint8_t)0x11 //
#define MX20340_REG_RX_DATA2            (uint8_t)0x12 //

#define MAX20340_LEFTEAR_I2C_ADDR 0x15
#define MAX20340_RIGHTEAR_I2C_ADDR 0x6a
#define MAX20340_I2C_FREQ 400

#define MAX20340_TEST
#define MAX20340_TEST_PIN 18

bool max20340ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value);
bool max20340ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len);
bool max20340WriteRegister(bitserial_handle handle, uint8 reg, uint8 value);
bool max20340WriteRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len);//len<20
bitserial_handle max20340Enable(void);
void max20340Disable(bitserial_handle handle);
void singlebus_itr_handler(Task task, MessageId id, Message msg);
void singlebus_key_itr_handler(Task task, MessageId id, Message msg);
void singlebus_itr_process(void);

#endif // MAX20340_H
