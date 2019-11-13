#include <panic.h>
#include <stdio.h>
#include <vmal.h>
#include <Panic.h>
#include <stdlib.h>
#include <message.h>
#include <input_event_manager.h>

#include "pio_common.h"
#include "bitserial_api.h"
#include "av_headset.h"

#include "apollo.h"

static void apollo_task_handler(Task appTask, MessageId id, Message msg);
static void apollo_send_cmd(uint32 cmd, bitserial_handle handle);
static void apollo_send_data(uint8* data, int length, bitserial_handle handle);
static int apollo_read_fb(uint8 *ret, uint32 length, bitserial_handle handle);
static int apollo_fb(uint8 *ret, uint32 length);
static void write_image_param(bitserial_handle handle, uint32 addr, uint32 size, uint32 crc);
static int transfer_image(void);
static void read_fw_version(void);
static const uint8* map_image(void);
static void release_image(void);
static void start_boot_mode_2(void);
static void start_boot_mode_1(void);
static int  transfer_image_3(void);
static int  start_new_image_s1(void);
static void wait_for_int_low(int delay);
static void apollo_init_handler(MessageId id, Message msg);
static void apollo_upgrade_handler(Task appTask, MessageId id, Message msg);
static void apollo_common_handler(Task appTask, MessageId id, Message msg);

/******* private defination *******/
#define GET_INT (PioGet32Bank(0) & (1<<APOLLO_INT_IO))
#define INT_IS_LOW (0 == GET_INT)
#define INT_IS_HIGH (1 == GET_INT)
#define WAIT_INT_LOW while(INT_IS_HIGH){}
#define WAIT_INT_HIGH while(INT_IS_LOW){}
#define MAKE_APOLLO_MESSAGE(TYPE) TYPE##_T *message = PanicUnlessNew(TYPE##_T)
#define UPGRADE_IMG_FILE_NAME "V1.00.5_upgrade.bin"

#define UNUSED2(a, b) (void)(a), (void)(b)
#define ENABLE_APOLLO_DEBUG (1)
#if (ENABLE_APOLLO_DEBUG == 1)
#define APOLLO_DBG_LOG8(fmt, p) DEBUG_LOG("Apollo: " fmt ": (hex) %x %x %x %x %x %x %x %x", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7])
#define APOLLO_DBG_LOG(...) DEBUG_LOG("Apollo: " __VA_ARGS__)
#else
#define APOLLO_DBG_LOG8(fmt, p) UNUSED(p)
#define APOLLO_DBG_LOG(...)
#endif

/******* static variables *******/
static ApolloTaskData apolloTaskData = {.data = {.handler = apollo_task_handler}};
static Task apolloTask = &(apolloTaskData.data);

static uint32 upgrade_remain_bytes;
static uint32 upgrade_offset;
static uint8* apollo_img_fptr = NULL;
static uint8* apollo_img_header;
static uint32 apollo_up_img_ver;
static uint32 apollo_fw_ver = -1;

static volatile uint8 apollo_state = APOLLO_STATE_INIT_IO;

/*
 * APIs
 */

/* init */
void apollo_int_io_init(void) {
    APOLLO_DBG_LOG("initing");
    apollo_state = APOLLO_STATE_INIT_IO;
    PioSetMapPins32Bank(0, (1<<APOLLO_INT_IO), (1<<APOLLO_INT_IO));
    PioSetDir32Bank(0, (1<<APOLLO_INT_IO), (0<<APOLLO_INT_IO));
    InputEventManagerRegisterTask(apolloTask, APOLLO_INT_IO);
    wait_for_int_low(10);
}

/* reset */
void apollo_reset(void) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = APOLLO_CMD_RESET;
    MessageSend(apolloTask, APOLLO_CMD_RESET, message);
}

int apolloGetStatus(void) { return 0; }

/*
 * static functions
 */

/* apollo initialize handler */
static void apollo_init_handler(MessageId id, Message msg)
{
    switch (id) {
        case MESSAGE_PIO_CHANGED:
        {
            if (INT_IS_LOW) {
                uint32 feedback[2];
                int ret = apollo_fb((uint8*)feedback, 8);

                if (ret) {
                    uint8 *fb = (uint8*)feedback;
                    APOLLO_DBG_LOG8("init fail", fb);
                    apollo_state = APOLLO_STATE_ERR;
                    break;
                }

                if ((!ret) && (0x11 == feedback[0]))
                    APOLLO_DBG_LOG("wakeup during initing...");


                if (APOLLO_STATE_INIT_RD_FW_VER == apollo_state) {
                    if (0x06 == feedback[0]) {
                        uint8 *fb = (uint8*)feedback;
                        APOLLO_DBG_LOG8("init read fw", fb);
                        APOLLO_DBG_LOG("init ok.");

#if 0
                        apollo_state = APOLLO_STATE_ENTERING_BOOT_MODE_S1;
                        start_boot_mode_1();
#else
                        apollo_state = APOLLO_STATE_IDLE;
#endif
                    }
                    else if (0x11 == feedback[0]) {
                        APOLLO_DBG_LOG("read fw again...");
                        read_fw_version();
                    }
                }

            }
            break;
        }
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = ((APOLLO_COMMAND_T*)msg);

            if (APOLLO_CMD_WAIT_INT_LOW == message->command) {
                if (APOLLO_STATE_INIT_IO == apollo_state) {
                    if (INT_IS_LOW) {
                        APOLLO_DBG_LOG("int init ok");
                        apollo_state = APOLLO_STATE_INIT_RD_FW_VER;
                        read_fw_version();
                    } else {
                        APOLLO_DBG_LOG("init wait int low timeout");
                        wait_for_int_low(2);
                    }
                }
            }
            break;
        }
    }
}

/* apollo upgrade handler */
static void apollo_upgrade_handler(Task appTask, MessageId id, Message msg)
{
    UNUSED2(appTask, msg);

    switch (id) {
        case MESSAGE_PIO_CHANGED:
        {
            if (0 == GET_INT) {
                switch (apollo_state) {
                    case APOLLO_STATE_ENTERING_BOOT_MODE_S1: {
                        apollo_state = APOLLO_STATE_ENTERING_BOOT_MODE_S2;
                        start_boot_mode_2();
                        break;
                    }
                    case APOLLO_STATE_ENTERING_BOOT_MODE_S3: {
                        uint32 feedback[2];
                        int ret = apollo_fb((uint8*)feedback, 8);

                        if (ret) {
                            APOLLO_DBG_LOG("read fw version fault: %04d", __LINE__);
                            apollo_state = APOLLO_STATE_ERR;
                            break;
                        }

                        if (0x06 == feedback[0]) {
                            // TODO: if the fw version is not good boot version;
                            uint8 *fb = (uint8*)feedback;
                            apollo_state = APOLLO_STATE_UPGRADE_S1;
                            APOLLO_DBG_LOG8("enter boot succ, ver", fb);

                            ///// test:
#if 1
                            apollo_state = APOLLO_STATE_IDLE;
                            break;
#else
                            if (start_new_image_s1()){
                                apollo_state = APOLLO_STATE_ERR;
                                APOLLO_DBG_LOG("upgrade img not found");
                            }
#endif
                        }
                        else if (0x11 == feedback[0]) {
                            APOLLO_DBG_LOG("read fw again...");
                            read_fw_version();
                        }

                        break;
                    }
                    case APOLLO_STATE_UPGRADE_S1: {
                        int remain_bytes_before = upgrade_remain_bytes;
                        int remain_bytes = transfer_image();
                        APOLLO_DBG_LOG("transfer bytes: %d, remain: %d", remain_bytes_before - remain_bytes, remain_bytes);
                        if (0 == remain_bytes)
                            apollo_state = APOLLO_STATE_UPGRADE_S2;
                        break;
                    }
                    case APOLLO_STATE_UPGRADE_S2: {
                        int ret = transfer_image_3();
                        if (!ret) {
                            apollo_state = APOLLO_STATE_INIT_IO;
                            release_image();
                            wait_for_int_low(10);
                        } else {
                            apollo_state = APOLLO_STATE_ERR;
                        }
                        break;
                    }
                }
            }
        }
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = (APOLLO_COMMAND_T*)msg;

            switch (message->command) {
                case APOLLO_CMD_WAIT_INT_LOW: {
                    if (APOLLO_STATE_ENTERING_BOOT_MODE_S2 == apollo_state) {
                        if (INT_IS_LOW) {
                            apollo_state = APOLLO_STATE_ENTERING_BOOT_MODE_S3;
                            read_fw_version();
                        } else {
                            APOLLO_DBG_LOG("enter boot mode wait int low timeout");
                            wait_for_int_low(5);
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
}

static void apollo_common_handler(Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    switch (id) {
        case MESSAGE_PIO_CHANGED:
        {
            if (0 == GET_INT) {
                switch (apollo_state) {
                    case APOLLO_STATE_IDLE: {
                        uint32 feedback;

                        if (apollo_fb((uint8*)&feedback, 4)) {
                            APOLLO_DBG_LOG("read fw version fault: %04d", __LINE__);
                            break;
                        }

                        if (0x11 == feedback)
                            APOLLO_DBG_LOG("wakeup");
                        else
                            APOLLO_DBG_LOG("fake wakeup: 0x%x", feedback);

                        break;
                    }
                    case APOLLO_STATE_READING_FW_VER: {
                        uint32 feedback[2];
                        int ret = apollo_fb((uint8*)feedback, 8);

                        if (ret) {
                            APOLLO_DBG_LOG("read fw version fault: %04d", __LINE__);
                            apollo_state = APOLLO_STATE_ERR;
                            break;
                        }

                        if (0x06 == feedback[0]) {
                            uint8 *fb = (uint8*)feedback;
                            apollo_fw_ver = feedback[1];
                            apollo_state = APOLLO_STATE_IDLE;

                            APOLLO_DBG_LOG8("fw ver", fb);
                        }
                        else if (0x11 == feedback[0]) {
                            APOLLO_DBG_LOG("read fw again...");
                            read_fw_version();
                        }
                    }
                }
            }
            break;
        }
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = (APOLLO_COMMAND_T*)msg;

            switch (message->command) {
                case APOLLO_CMD_READ_FW_VER: {
                    read_fw_version();
                    break;
                }
                case APOLLO_CMD_RESET: {
                    APOLLO_DBG_LOG("dummy reset");
                    break;
                }
            }
            break;
        }
    }
}

/* apollo task handler */
static void apollo_task_handler(Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);

    if (apollo_state < APOLLO_STATE_INIT_END)
        apollo_init_handler(id, msg);
    else if (apollo_state < APOLLO_STATE_UPGRADE_END)
        apollo_upgrade_handler(appTask, id, msg);
    else
        apollo_common_handler(appTask, id, msg);
}

/*
 * fw upgrade process
*/

/* enter boot mode */
int apollo_start_boot_mode(void) {
    if (APOLLO_STATE_IDLE != apollo_state) {
        APOLLO_DBG_LOG("busy, state: %d", apollo_state);
        return -1;
    }
    apollo_state = APOLLO_STATE_ENTERING_BOOT_MODE_S1;
    start_boot_mode_1();
    return 0;
}

static void start_boot_mode_1(void) {
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    apollo_send_cmd(0x10, handle);
    hwi2cClose(handle);
}

static void start_boot_mode_2(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    apollo_send_cmd(0x09, handle);
    hwi2cClose(handle);

    wait_for_int_low(80);
}

/* start new image */
void apollo_start_new_image(void) {
    if (APOLLO_STATE_IDLE == apollo_state)
    {
        if (0 == start_new_image_s1())
            apollo_state = APOLLO_STATE_UPGRADE_S1;
        else
            APOLLO_DBG_LOG("map image fail: " UPGRADE_IMG_FILE_NAME);
    }
    else
        APOLLO_DBG_LOG("busy, state: %d", apollo_state);
}

static int start_new_image_s1(void)
{
    const apollo_image_header_t *header = (const apollo_image_header_t*) map_image();

    if (NULL == header) return -1;

    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));

    upgrade_offset = 0;
    upgrade_remain_bytes = header->image_len;
    apollo_img_fptr = (uint8*)&(header->image);
    apollo_img_header = (uint8*)header;
    apollo_up_img_ver = header->version;

    write_image_param(handle, header->image_addr, upgrade_remain_bytes, header->crc);
    apollo_send_cmd(0x02, handle);

    hwi2cClose(handle);

    return 0;
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


static int transfer_image(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));

    if (upgrade_remain_bytes > 0)
    {
        uint32 trans_bytes = upgrade_remain_bytes > 112 ? 112 : upgrade_remain_bytes;

        apollo_send_data(apollo_img_fptr + upgrade_offset, trans_bytes, handle);
        apollo_send_cmd(0x03, handle);

        upgrade_remain_bytes -= trans_bytes;
        upgrade_offset += trans_bytes;
    }

    hwi2cClose(handle);

    return upgrade_remain_bytes;
}

static int transfer_image_3(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    uint32 ret = 0;

    apollo_read_fb((uint8*)&ret, 4, handle);
    apollo_send_cmd(0x04, handle);

    hwi2cClose(handle);

    return (3 == ret) ? 0 : -1;
}

/*
 * read firmware version
*/
int apollo_read_fw_version(void) {
    if (APOLLO_STATE_IDLE != apollo_state) {
        APOLLO_DBG_LOG("busy, state: %d", apollo_state);
        return -1;
    }

    apollo_state = APOLLO_STATE_READING_FW_VER;
    read_fw_version();
    return 0;
}

static void read_fw_version(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    apollo_send_cmd(0x06, handle);
    hwi2cClose(handle);
}

/*
 * general utilities
*/
static void wait_for_int_low(int delay) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = APOLLO_CMD_WAIT_INT_LOW;
    MessageSendLater(apolloTask, APOLLO_MESSAGE_CMD, message, delay);
}

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

static int apollo_fb(uint8 *fb, uint32 length)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    int ret = apollo_read_fb((uint8*)fb, length, handle);
    hwi2cClose(handle);
    return ret;
}

static int apollo_read_fb(uint8 *ret, uint32 length, bitserial_handle handle)
{
    uint8 reg = APOLLO_FEEDBACK;
    int r = -1;

    if (handle) r = hwi2cRead(handle, &reg, 1, ret, length);

    return r;
}

static const uint8* map_image(void) {
    const char apollo_image_name[] = UPGRADE_IMG_FILE_NAME;

    FILE_INDEX idx = FileFind(FILE_ROOT, apollo_image_name, strlen(apollo_image_name));

    return (FILE_NONE == idx) ? NULL : FileMap(idx, 0, FILE_MAP_SIZE_ALL);
}

static void release_image(void) {
    FileUnmap(apollo_img_header);
}
