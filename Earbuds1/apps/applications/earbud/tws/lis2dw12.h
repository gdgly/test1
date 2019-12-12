#ifndef LIS2DW12_H
#define LIS2DW12_H
#include <stdio.h>
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <input_event_manager.h>
#include "../av_headset.h"
#include "../av_headset_log.h"

#define LIS2DW12_I2C_ADDR 0x19
#ifdef CONFIG_BOARD_V2
#undef  LIS2DW12_I2C_ADDR
#define LIS2DW12_I2C_ADDR 0x18
#endif
#define LIS2DW12_I2C_FREQ 400

extern uint8  lis2dw12Runing;              // poweroff后，设置为 0, 启动运行为1

bool lis2dw12ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value);
bool lis2dw12WriteRegister(bitserial_handle handle, uint8 reg, uint8 value);
bool lis2dw12ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len);
bitserial_handle lis2dw12Enable(void);
void lis2dw12Disable(bitserial_handle handle);

#endif // LIS2DW12_H
