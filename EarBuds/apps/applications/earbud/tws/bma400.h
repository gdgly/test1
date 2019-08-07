#ifndef BMA400_H
#define BMA400_H
#define HAVE_BMA400
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <hydra_macros.h>
#include <input_event_manager.h>
#include "../av_headset.h"
#include "../av_headset_log.h"
#include "../av_headset_accelerometer.h"
#include "public.h"

#define BMA400_I2C_ADDR 0x14
#define BMA400_I2C_FREQ 100
#define BMA400_ITR_PIN 7

typedef struct{
    uint8 reg;
    uint8 mask;
    uint8 value;
}bma400_str;

bool BMA400ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value);
bool BMA400WriteRegister(bitserial_handle handle, uint8 reg, uint8 value);
bitserial_handle BMA400Enable(void);
void BMA400Disable(bitserial_handle handle);
void BMA400_itr_handler(Task task, MessageId id, Message msg);
int BMA400Power(bool isOn);

#endif // BMA400_H
