#ifndef UCS146E0_H
#define UCS146E0_H
#include <stdio.h>
#include <bitserial_api.h>
#include <panic.h>
#include <pio.h>
#include <input_event_manager.h>
#include "../av_headset.h"
#include "../av_headset_log.h"
#include "../av_headset_proximity.h"

//#define HAVE_UCS146E0
#define UCS146E0_ITR_PIN 8
void Ucs146e0_init(void);
int Ucs146e0_GetStatus(void);
int Ucs146e0_statcheck(void);
int Ucs146e0Power(bool isOn);
void Ucs146e0_get_crosstalk_init(void);//260ms后读取crosstalk值
unsigned short Ucs146e0_get_crosstalk_calvalue(void);
void Ucs146e0_get_ps_cal_init(unsigned short cal_value);//初始化完需200ms延时后读取ps值
unsigned short Ucs146e0_get_ps_calvalue(void);

#define UCS146E0_I2C_ADDR 0x38
#define UCS146E0_I2C_FREQ 400

#define UCS146E0_HIGH_VALUE 1200
#define UCS146E0_HIGH_VALUE_H ((UCS146E0_HIGH_VALUE & 0xff00) >> 8)
#define UCS146E0_HIGH_VALUE_L ((UCS146E0_HIGH_VALUE & 0xff))

#define UCS146E0_LOW_VALUE 750
#define UCS146E0_LOW_VALUE_H ((UCS146E0_LOW_VALUE & 0xff00) >> 8)
#define UCS146E0_LOW_VALUE_L ((UCS146E0_LOW_VALUE & 0xff))

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

typedef struct{
    uint8 reg;
    uint8 value;
}ucs146e0_str;

#endif // UCS146E0_H
