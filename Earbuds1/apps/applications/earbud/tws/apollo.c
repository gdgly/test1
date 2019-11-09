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

#define GET_INT (PioGet32Bank(0) & (1<<APOLLO_INT_IO))
#define WAIT_INT_LOW while(0 != GET_INT){}
#define WAIT_INT_HIGH while (0 == GET_INT){}
#define MAKE_APOLLO_MESSAGE(TYPE) TYPE##_T *message = PanicUnlessNew(TYPE##_T)

static void apollo_task_handler(Task appTask, MessageId id, Message msg);
static void apollo_send_cmd(uint32 cmd, bitserial_handle handle);
static void apollo_send_data(uint8* data, int length, bitserial_handle handle);
static void init(void);
static void start_boot_mode(void);
static void write_image_param(bitserial_handle handle, uint32 addr, uint32 size, uint32 crc);
static int  start_new_image(void);
static void transfer_image_1(void);
static void transfer_image_2(void);
static int  transfer_image_3(void);
static int  read_fw_version(uint8 *fw_ver);
static const uint8* map_image(void);
static void reset(void);
static void delay_ms(uint16 v_msec_16);


static ApolloTaskData apolloTaskData = {.data = {.handler = apollo_task_handler}};
static Task apolloTask = &(apolloTaskData.data);

static uint32 upgrade_remain_bytes;
static uint32 upgrade_offset;
static uint8* apollo_img_fptr = NULL;
static uint32 fw_version;

/*
 * APIs
 */

void apollo_int_io_init(void) {
    PioSetMapPins32Bank(0, (1<<APOLLO_INT_IO), (1<<APOLLO_INT_IO));
    PioSetDir32Bank(0, (1<<APOLLO_INT_IO), (0<<APOLLO_INT_IO));
    DEBUG_LOG("apollo_int_io_init");
}

void apollo_init(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x03;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_start_boot_mode(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x05;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_start_new_image(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x06;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_transfer_image(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x07;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_read_fw_version(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x01;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

void apollo_reset(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x12;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

int apolloGetStatus(void) { return 0; }

/*
 * static functions
 */

/* apollo task handler */
static void apollo_task_handler(Task appTask, MessageId id, Message msg)
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
                case 0x01: {
                    uint8 ver[8];
                    read_fw_version(ver);
                    DEBUG_LOG("fwv: %d, %d, %d, %d, %d, %d, %d, %d",
                              ver[0], ver[1], ver[2], ver[3], ver[4], ver[5], ver[6], ver[7]);
                    break;
                }
                case 0x03:
                    init();
                    break;
                case 0x05:
                    start_boot_mode();
                    break;
                case 0x06: {
                    if (start_new_image())
                        DEBUG_LOG("start new image fail!");
                    else
                        DEBUG_LOG("start new image succ!");
                    break;
                }
                case 0x07:
                    transfer_image_1();
                    break;
                case 0x08:
                    transfer_image_2();
                    break;
                case 0x09:{
                    if (transfer_image_3())
                        DEBUG_LOG("apollo upgrade fail!");
                    else
                        DEBUG_LOG("apollo upgrade succ!");
                    break;
                }
                case 0x12:
                    reset();
                    break;
            }
            break;
        }
    }
}

/* general utilities */
static void apollo_send_cmd(uint32 cmd, bitserial_handle handle)
{
    if (0 != handle) {
        uint32 cmd_buf_w[2];
        uint8 *cmd_buf = (uint8*)cmd_buf_w;

        cmd_buf[3] = APOLLO_COMMAND;
        cmd_buf_w[1] = cmd;

        hwi2cWrite(handle, cmd_buf + 3, 5);
    }
}

static void apollo_send_data(uint8* data, int length, bitserial_handle handle)
{
    uint8 buf[112 + 4 + 4];
    uint32 *data_w = (uint32*)buf;

    if (0 != handle) {
        data_w[1] = length;
        buf[3] = APOLLO_DATA;
        memcpy(buf + 8, data, length);
        hwi2cWrite(handle, buf + 3, length + 4 + 1);
    }
}

static void reset(void) {

}

static int apollo_read_feedback(uint8 *ret, uint32 length, bitserial_handle handle)
{
    uint8 reg = APOLLO_FEEDBACK;
    int r = -1;

    if (handle) r = hwi2cRead(handle, &reg, 1, ret, length);

    return r;
}

static void delay_ms(uint16 v_msec_16)
{
    uint32 v_delay = VmGetTimerTime() + (v_msec_16 * 1000) + 1;
    while (((int32)(VmGetTimerTime() - v_delay)) < 0);
}

static const uint8* map_image(void) {
    const char apollo_image_name[] = "update_binary_apollo2_blue.bin";

    FILE_INDEX idx = FileFind(FILE_ROOT, apollo_image_name, strlen(apollo_image_name));

    return (FILE_NONE == idx) ? NULL : FileMap(idx, 0, FILE_MAP_SIZE_ALL);
}

/* peripheral init */
static void init(void)
{
    WAIT_INT_LOW;
}

/* sw upgrade process */
static void start_boot_mode(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));

    WAIT_INT_LOW;
    apollo_send_cmd(0x00, handle);

    WAIT_INT_HIGH;
    apollo_send_cmd(0x10, handle);

    WAIT_INT_LOW;
    apollo_send_cmd(0x09, handle);

    delay_ms(3);

    WAIT_INT_LOW;
    hwi2cClose(handle);
}

static void write_image_param(bitserial_handle handle,
                              uint32 addr, uint32 size, uint32 crc)
{
    if (0 != handle) {
        uint32 cmd_buf_w[4] = {APOLLO_DATA, addr, size, crc};
        uint8 *cmd_buf = (uint8*) cmd_buf_w;
        cmd_buf[3] = APOLLO_DATA;
        hwi2cWrite(handle, cmd_buf + 3, 12 + 1);
    }
}

static int start_new_image(void)
{
    uint32 ret = 0;

    const apollo_image_header_t *header = (const apollo_image_header_t*) map_image();
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));

    if (NULL == header) return -1;

    upgrade_offset = 0;
    upgrade_remain_bytes = header->image_len;
    apollo_img_fptr = (uint8*)&(header->image);
    fw_version = header->version;

    WAIT_INT_LOW;
    apollo_send_cmd(0x00, handle);

    WAIT_INT_HIGH;
    write_image_param(handle, header->image_addr, upgrade_remain_bytes, header->crc);
    apollo_send_cmd(0x02, handle);

    WAIT_INT_LOW;
    apollo_read_feedback((uint8*)&ret, 4, handle);

    hwi2cClose(handle);

    DEBUG_LOG("apollo image: addr: %d, len: %d, crc: 0x%x",
              header->image_addr, header->image_len, header->crc);

    return (0x02 == ret) ? 0 : -1;
}

static void transfer_image_1(void)
{
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = 0x08;
    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

static void transfer_image_2(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);

    if (upgrade_remain_bytes > 0)
    {
        /* prepare data packet */
        uint32 trans_bytes = upgrade_remain_bytes > 112 ? 112 : upgrade_remain_bytes;

        WAIT_INT_LOW;
        apollo_send_cmd(0x00, handle);

        WAIT_INT_HIGH;
        apollo_send_data(apollo_img_fptr + upgrade_offset, trans_bytes, handle);
        apollo_send_cmd(0x03, handle);

        upgrade_remain_bytes -= trans_bytes;
        upgrade_offset += trans_bytes;

        message->command = 0x08;
    } else {
        message->command = 0x09;
    }

    hwi2cClose(handle);

    MessageSend(apolloTask, APOLLO_MESSAGE_CMD, message);
}

static int transfer_image_3(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    uint32 ret = 0;

    WAIT_INT_LOW;
    apollo_read_feedback((uint8*)&ret, 4, handle);

    DEBUG_LOG("transfer_image ret: %d, restarting apollo", ret);

    apollo_send_cmd(0x00, handle);

    WAIT_INT_HIGH;
    apollo_send_cmd(0x04, handle);

    WAIT_INT_LOW;

    hwi2cClose(handle);

    return (3 == ret) ? 0 : -1;
}

/* read firmware version */
static int read_fw_version(uint8 *fw_ver)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    uint8 ret;

    WAIT_INT_LOW;
    apollo_send_cmd(0x00, handle);

    WAIT_INT_HIGH;
    apollo_send_cmd(0x06, handle);

    WAIT_INT_LOW;
    ret = apollo_read_feedback(fw_ver, 8, handle);

    hwi2cClose(handle);

    return ret;
}
