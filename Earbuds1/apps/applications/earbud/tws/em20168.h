#ifndef EM20168_H
#define EM20168_H
#include <stdio.h>
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <input_event_manager.h>
#include "../av_headset.h"
#include "../av_headset_log.h"
#include "../av_headset_proximity.h"

//#define EM20168_CAL_OFFSET_VALUE //校准0x24寄存器
//#define EM20168_KEY_ITR_TEST  //打开按键中断
//#define EM20168_SEND_MSG  //打开发送入耳出耳消息
#define EM20168_I2C_ADDR 0x24
#define EM20168_I2C_FREQ 400
#define EM20168_KEY_ITR_PIN 18

//#define EM20168_HIGH_VALUE 0x200
#define EM20168_HIGH_VALUE 0x3300
#define EM20168_HIGH_VALUE_H ((EM20168_HIGH_VALUE & 0xff00) >> 8)
#define EM20168_HIGH_VALUE_L ((EM20168_HIGH_VALUE & 0xff))

//#define EM20168_LOW_VALUE 0x80
#define EM20168_LOW_VALUE 0x1080
#define EM20168_LOW_VALUE_H ((EM20168_LOW_VALUE & 0xff00) >> 8)
#define EM20168_LOW_VALUE_L ((EM20168_LOW_VALUE & 0xff))

typedef struct{
    uint8 reg;
    uint8 value;
}em20168_str;

enum proximity_states
{
    proximity_state_unknown,
    proximity_state_in_proximity,
    proximity_state_not_in_proximity
};
struct __proximity_state
{
    /*! The sensor proximity state */
    enum proximity_states proximity;
};

bool EM20168ReadRegister(bitserial_handle handle, uint8 reg,  uint8 *value);
bool EM20168WriteRegister(bitserial_handle handle, uint8 reg, uint8 value);
bitserial_handle EM20168Enable(void);
void EM20168Disable(bitserial_handle handle);
void EM20168_itr_handler(Task task, MessageId id, Message msg);
void EM20168_itr_read_reg(Task task, MessageId id, Message msg);
void EM20168_keytest_itr_handler(Task task, MessageId id, Message msg);

#endif // EM20168_H
