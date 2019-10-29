#include <panic.h>
#include <stdio.h>
#include <vmal.h>
#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "pio_common.h"
#include "bitserial_api.h"
#include "av_headset.h"

#include "apollo.h"

#define APOLLO_CHIPADDR         (0x10) // (0x68)
#define APOLLO_I2C_FREQ         (400)

#define APOLLO_INT_IO           (0x06)

#define APOLLO_GET_SW_VER       (0x06) // get software version

#define APOLLO_FEEDBACK         (0x00)
#define APOLLO_COMMAND          (0x80)
#define APOLLO_DATA             (0x84)

#define APOLLO_UPDATE_MESSAGE_BASE (0x11)

static void apollo_upgrade_handler(Task appTask, MessageId id, Message msg);
static void upgrade(void);
static uint32 get_int(void);

static uint8 apollo_buf[128];

#define  APOLLO_UPGRADE_MESSAGE_BASE (0x11)

#define MAKE_APOLLO_MESSAGE(TYPE) TYPE##_T *message = PanicUnlessNew(TYPE##_T)

typedef struct {
    uint32 command;
} APOLLO_COMMAND_T;

typedef enum apollo_upgrade_state {
    APOLLO_UPGRADE_STAGE1 = APOLLO_UPGRADE_MESSAGE_BASE,
    APOLLO_UPGRADE_STAGE2,
    APOLLO_UPGRADE_CMD,
    APOLLO_MESSAGE_CMD
} apollo_upgrade_state_t;

static ApolloTaskData apolloTaskData =
{
    .data = {.handler = apollo_upgrade_handler},
};

static Task apolloTask = &(apolloTaskData.data);

void apollo_int_io_init(void){
    PioSetMapPins32Bank(0, (1<<APOLLO_INT_IO), (1<<APOLLO_INT_IO));
    PioSetDir32Bank(0, (1<<APOLLO_INT_IO), (0<<APOLLO_INT_IO));
    DEBUG_LOG("apollo_int_io_init");
#ifdef CONFIG_KEY_INTERRUPT
    InputEventManagerInit(&gKeyData.task, InputEventActions,
                          sizeof(InputEventActions), &InputEventConfig);
    gKeyData.keyvalue = (PioGet32Bank(0) & (1<<APOLLO_INT_IO));
    PanicNotZero(PioSet32Bank(0, (1<<APOLLO_INT_IO), (1<<APOLLO_INT_IO)));
    InputEventManagerRegisterTask(&gKeyData.task, APOLLO_INT_IO);
#endif
}

static uint32 get_int(void){
#ifdef CONFIG_KEY_INTERRUPT
    return gKeyData.keyvalue;
#else
    return (PioGet32Bank(0) & (1<<APOLLO_INT_IO));
#endif
}

static void read_sw_version(void) {
    uint8 reg;
    bitserial_handle handle = hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ);

    while (get_int() == 0);

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x06;
    hwi2cWrite(handle, apollo_buf, 5);

    while (get_int() != 0);

    reg = APOLLO_FEEDBACK;
    hwi2cRead(handle, &reg, 1, apollo_buf, 8);

    DEBUG_LOG("sw version: %x, %x, %x, %x, %x, %x, %x, %x",
              apollo_buf[0], apollo_buf[1], apollo_buf[2], apollo_buf[3],
              apollo_buf[4], apollo_buf[5], apollo_buf[6], apollo_buf[7]);

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x00;
    hwi2cWrite(handle, apollo_buf, 5);

    while (get_int() == 0);

    hwi2cClose(handle);
}

static void check_wakeup(void) {
    uint8 reg;
    bitserial_handle handle = hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ);

    if (get_int() != 0) {
        DEBUG_LOG("no wakeup interrupt");
        return;
    }

    reg = APOLLO_FEEDBACK;
    hwi2cRead(handle, &reg, 1, apollo_buf, 4);

    DEBUG_LOG("wakeup data: %x, %x, %x, %x", apollo_buf[0], apollo_buf[1], apollo_buf[2], apollo_buf[3]);

    hwi2cClose(handle);
}

static void upgrade(void) {
    uint8 reg;

    bitserial_handle handle = hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ);

    while (get_int() == 0);

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x10;       /*擦除头部准备进入升级模式*/
    hwi2cWrite(handle, apollo_buf, 5);

    while (get_int() != 0);

    reg = APOLLO_FEEDBACK;
    hwi2cRead(handle, &reg, 1, apollo_buf, 8);

    DEBUG_LOG("sversion: %x, %x, %x, %x",
              apollo_buf[4], apollo_buf[5], apollo_buf[6], apollo_buf[7]);

    if (0x10 != apollo_buf[0]) {
        DEBUG_LOG("enter boot mode fail: 0x%x, return!", apollo_buf[0]);
        return;
    }

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x00;
    hwi2cWrite(handle, apollo_buf, 5);
    while (get_int() == 0);

#if 0
    DEBUG_LOG("======== reset apollo ======");

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x09;
    hwi2cWrite(handle, apollo_buf, 5);

    while (get_int() != 0);
#endif

    hwi2cClose(handle);
}


void apollo_read_sw_version(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x01;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_upgrade(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x02;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_init(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x03;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_check_wakeup(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x04;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

static void init(void) {
    uint8 reg;
    bitserial_handle handle = hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ);

    if(get_int() != 0)
    {
        reg = APOLLO_COMMAND;
        apollo_buf[0] = reg;
        apollo_buf[1] = 0x06;
        hwi2cWrite(handle, apollo_buf, 5);

        while (get_int() != 0);

        reg = APOLLO_FEEDBACK;
        hwi2cRead(handle, &reg, 1, apollo_buf, 8);
    }

    reg = APOLLO_COMMAND;
    apollo_buf[0] = reg;
    apollo_buf[1] = 0x00;
    hwi2cWrite(handle, apollo_buf, 5);
    while (get_int() == 0);

    hwi2cClose(handle);
}

static void apollo_upgrade_handler(Task appTask, MessageId id, Message msg)
{
    UNUSED(msg);
    UNUSED(appTask);
    switch (id) {
        case APOLLO_UPGRADE_STAGE1:
            DEBUG_LOG("1");
            break;
        case APOLLO_UPGRADE_STAGE2:
            DEBUG_LOG("2");
            break;
        case APOLLO_UPGRADE_CMD:
            DEBUG_LOG("3");
            break;
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = (APOLLO_COMMAND_T*)msg;
            switch (message->command) {
            case 0x01:
                DEBUG_LOG("apollo get sw version.");
                init();
                read_sw_version();
                break;
            case 0x02:
                DEBUG_LOG("apollo upgrade.");
                upgrade();
                break;
            case 0x03:
                init();
                break;
            case 0x04:
                check_wakeup();
                break;
            }
            break;
        }
    }
}

