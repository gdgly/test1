#ifndef BMA400_H
#define BMA400_H

#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <hydra_macros.h>
#include <input_event_manager.h>

#include "../av_headset.h"
#include "../av_headset_log.h"
#include "../av_headset_accelerometer.h"
#define HAVE_BMA400

#define BMA400_I2C_ADDR 0x14
#define BMA400_I2C_FREQ 100
#define BMA400_I2C_DATA_PIN 20
#define BMA400_I2C_CLK_PIN 21
#define BMA400_ITR_PIN 7


bool BMA400ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len);
bool BMA400WriteRegister(bitserial_handle handle, uint8 *msg, uint8 len);
bitserial_handle BMA400Enable(void);
void BMA400_init(bitserial_handle handle);
void BMA400_itr_handler(Task task, MessageId id, Message msg);

#endif // BMA400_H
