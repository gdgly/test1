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

#define MAX20340_LEFTEAR_I2C_ADDR 0x15
#define MAX20340_RIGHTEAR_I2C_ADDR 0x6a
#define MAX20340_I2C_FREQ 400

bool max20340ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value);
bool max20340ReadRegister_withlen(bitserial_handle handle, uint8 reg,  uint8 *value, uint8 len);
bool max20340WriteRegister(bitserial_handle handle, uint8 reg, uint8 value);
bitserial_handle max20340Enable(void);
void max20340Disable(bitserial_handle handle);
void singlebus_itr_handler(Task task, MessageId id, Message msg);

#endif // MAX20340_H
