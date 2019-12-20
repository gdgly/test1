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
static int apollo_send_cmd(uint32 cmd, bitserial_handle handle);
static void apollo_send_data(uint8* data, int length, bitserial_handle handle);
static int apollo_read_fb(uint8 *ret, uint32 length, bitserial_handle handle);
static int apollo_fb(uint8 *ret, uint32 length);
static void write_image_param(bitserial_handle handle, uint32 addr, uint32 size, uint32 crc);
static int transfer_image(void);
static int read_fw_version(void);
static const uint8* map_image(void);
static void release_image(void);
static void start_boot_mode_1(void);
static int  transfer_image_3(void);
static int  start_new_image_s1(void);
static void wait_for_int_low(int delay);
static void apollo_init_handler(MessageId id, Message msg);
static void apollo_upgrade_handler(MessageId id, Message msg);
static void apollo_common_handler(MessageId id, Message msg);
static bool should_upgrade(void);
static int apollo_check_and_start_upgrade(void);
static int check_fw_ver(uint32 fw_ver);
static void wait_for_timeout(int delay);
static void apollo_init_finish(void);
static int send_successful_upgrade(void);


/****** debug ******/
#define ENABLE_APOLLO_DEBUG (1)
#if (ENABLE_APOLLO_DEBUG == 1)
#define APOLLO_DBG_LOG8(fmt, p) DEBUG_LOG("Apollo: " fmt \
                        ": (hex) %x %x %x %x %x %x %x %x", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7])
#define APOLLO_DBG_LOG(...) DEBUG_LOG("Apollo: " __VA_ARGS__)
#else
#define APOLLO_DBG_LOG8(fmt, p) UNUSED(p)
#define APOLLO_DBG_LOG(...)
#endif

/******* upgrade image file name ******/
#define UPGRADE_IMG_FILE_NAME "apollo2_blue.bin"

/******* private defination *******/
#define GET_INT         (PioGet32Bank(0) & (1<<APOLLO_INT_IO))
#define INT_IS_LOW      (0 == GET_INT)
#define INT_IS_HIGH     (1 == GET_INT)

#define IO_HIGH(APIO)   PioSet32Bank(PIO2BANK(APIO), PIO2MASK(APIO), PIO2MASK(APIO))
#define IO_LOW(APIO)    PioSet32Bank(PIO2BANK(APIO), PIO2MASK(APIO), 0)

#define MAKE_APOLLO_MESSAGE(TYPE) TYPE##_T *message = PanicUnlessNew(TYPE##_T)

/******* static variables *******/
static ApolloTaskData apolloTaskData = {.data = {.handler = apollo_task_handler}};
static Task apolloTask = &(apolloTaskData.data);

static uint32 upgrade_remain_bytes;
static uint32 upgrade_offset;
static uint8* apollo_img_fptr = NULL;
static uint8* apollo_img_header;
static uint32 apollo_up_img_ver;
static uint32 apollo_fw_ver = -1;

static apollo_wakeup_cb_t apollo_init_end_cb = NULL;
static apollo_wakeup_cb_t apollo_enter_boot_cb = NULL;
static apollo_wakeup_cb_t apollo_wakeup_cb = NULL;

static volatile uint8 apollo_state = APOLLO_STATE_UNINIT;

static int io_init_wait_int_low_to = 0;
static int io_init_wait_int_low_interrupt = 0;
static int io_init_i2c_rd_err_times = 0;
static int io_init_rd_fw_err_times = 0;
static int enter_boot_int_times = 0;
static int upgrade_times = 0;

/*
 * APIs
 */

void apollo_int_io_init(void) {
    uint32 bank = 0, mask = 0;

    APOLLO_DBG_LOG("initing");

    apollo_state = APOLLO_STATE_INIT_IO;
    io_init_wait_int_low_to = 0;
    io_init_i2c_rd_err_times = 0;
    io_init_rd_fw_err_times = 0;
    enter_boot_int_times = 0;
    io_init_wait_int_low_interrupt = 0;
    upgrade_times = 0;
    apollo_init_end_cb = apollo_check_and_start_upgrade;
    apollo_enter_boot_cb = apollo_start_new_image;

    // int io init
    bank = PIO2BANK(APOLLO_INT_IO);
    mask = PIO2MASK(APOLLO_INT_IO);
    PioSetMapPins32Bank(bank, mask, mask);
    PioSetDir32Bank(bank, mask, 0);
    PioSet32Bank(bank, mask, mask);
    PioSetWakeupStateBank( bank,  mask,  mask);
    PioSetDeepSleepEitherLevelBank( bank,  mask,  mask);
    InputEventManagerRegisterTask(apolloTask, APOLLO_INT_IO);

    // override io init
    bank = PIO2BANK(APOLLO_OVERRIDE_IO);
    mask = PIO2MASK(APOLLO_OVERRIDE_IO);
    PioSetMapPins32Bank(bank, mask, mask);
    PioSetDir32Bank(bank, mask, mask);

    // reset io init
    bank = PIO2BANK(APOLLO_RESET_IO);
    mask = PIO2MASK(APOLLO_RESET_IO);
    PioSetMapPins32Bank(bank, mask, mask);
    PioSetDir32Bank(bank, mask, mask);

    // reset apollo to enter app mode
    IO_LOW(APOLLO_OVERRIDE_IO);
    IO_LOW(APOLLO_RESET_IO);
    wait_for_timeout(10);
}

void register_apollo_wakeup_cb(apollo_wakeup_cb_t cb) {
    apollo_wakeup_cb = cb;
}

int apollo_start_boot_mode(void) {
    if (APOLLO_STATE_IDLE != apollo_state) {
        APOLLO_DBG_LOG("apollo_start_boot_mode: busy(state: %d)", apollo_state);
        return -1;
    }

    apollo_state = APOLLO_STATE_ENTERING_BOOT_MODE_S1;

    start_boot_mode_1();

    return 0;
}

int apollo_start_new_image(void) {
    if (APOLLO_STATE_IDLE != apollo_state) {
        APOLLO_DBG_LOG("apollo_start_new_image: busy(state: %d)", apollo_state);
        return -1;
    }

    int status = start_new_image_s1();

    if (0 == status) apollo_state = APOLLO_STATE_UPGRADE_S1;

    APOLLO_DBG_LOG("map image status: %d", status);

    return status;
}

int apollo_read_fw_version(void) {
    if (APOLLO_STATE_IDLE != apollo_state) {
        APOLLO_DBG_LOG("busy, state: %d", apollo_state);
        return -1;
    }

    apollo_state = APOLLO_STATE_READING_FW_VER;

    read_fw_version();

    return 0;
}

int apollo_sleep(void) {
    if (APOLLO_STATE_IDLE != apollo_state) return -1;

    apollo_state = APOLLO_STATE_ENTERING_SLEEP;
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    apollo_send_cmd(APOLLO_SLEEP, handle);
    hwi2cClose(handle);

    return 0;
}

int apollo_evoke(void) {
    if (APOLLO_STATE_SLEEP != apollo_state) return -1;

    apollo_state = APOLLO_STATE_AWAKING;
    IO_HIGH(APOLLO_INT_IO);
    wait_for_timeout(10);
    return 0;
}

/*Return 0 for Apollo startup success*/
int apolloGetStatus(void) {
    return !((apollo_state > APOLLO_STATE_INIT_END) && (apollo_state != APOLLO_STATE_ERR));
}

/*get Apollo version*/
void comGetApolloVer(uint8 *arr) {
    memcpy(arr, (uint8*)&apollo_fw_ver, 4);
}

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
                if(APOLLO_STATE_START_UP == apollo_state)
                {
                    apollo_state = APOLLO_STATE_INIT_IO;
                }else
                /* reading apollo fw version during start up. */
                if (APOLLO_STATE_INIT_RD_FW_VER == apollo_state) {
                    uint32 feedback[2];
                    int ret = apollo_fb((uint8*)feedback, 8);

                    if (ret) {
                        if (io_init_i2c_rd_err_times < 3) {
                            /* if read fw ver cause i2c err during boot up, we re-init again. */
                            APOLLO_DBG_LOG("init read feed back fail");
                            apollo_state = APOLLO_STATE_INIT_IO;
                            wait_for_int_low(50);
                            io_init_i2c_rd_err_times ++;
                        }
                        else {
                            /* i2c fail to read fw version several times, mark apollo as error. */
                            APOLLO_DBG_LOG("init fail");
                            apollo_state = APOLLO_STATE_ERR;
                            IO_LOW(APOLLO_OVERRIDE_IO);
                            apollo_init_finish();
                        }
                        break;
                    }

                    if (APOLLO_ACK_RD_FW_VER == feedback[0]) {
                        /* init success when we go in this block. */
                        IO_LOW(APOLLO_OVERRIDE_IO);
                        apollo_fw_ver = feedback[1];
                        APOLLO_DBG_LOG("init ok, fw ver: 0x%x.", apollo_fw_ver);
                        apollo_state = APOLLO_STATE_IDLE;
                        apollo_init_finish();
                        if (apollo_init_end_cb) apollo_init_end_cb();
                    }
                    else if (APOLLO_ACK_WAKEUP == feedback[0]) {
                        /* an wakeup mixed with fw version read event, read fw ver again. */
                        APOLLO_DBG_LOG("wakeup during init, read fw again...");
                        if (apollo_wakeup_cb) apollo_wakeup_cb();
                        read_fw_version();
                    } else {
                        if (io_init_rd_fw_err_times < 3) {
                            /* fw ver read err during boot up, try again. */
                            APOLLO_DBG_LOG("read fw err, try again");
                            read_fw_version();
                            io_init_rd_fw_err_times ++;
                        } else {
                            /* finally read fw version fail, mark apollo as error. */
                            apollo_state = APOLLO_STATE_ERR;
                            IO_LOW(APOLLO_OVERRIDE_IO);
                            apollo_init_finish();
                        }
                    }
                }
            }
            break;
        }
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = ((APOLLO_COMMAND_T*)msg);

            /* waiting for init gap end */
            if (APOLLO_CMD_WAIT_INT_LOW == message->command) {
                if (APOLLO_STATE_INIT_IO == apollo_state) {
                    if (INT_IS_LOW) {
                        APOLLO_DBG_LOG("int init ok");
                        apollo_state = APOLLO_STATE_INIT_RD_FW_VER;
                        if(read_fw_version()) {
                            /* 老开发板如果没有连接apollo，读版本号时i2c操作会失败，当失败时也要发送结束消息，否则init过程无法走完. */
                            apollo_state = APOLLO_STATE_ERR;
                            IO_LOW(APOLLO_OVERRIDE_IO);
                            apollo_init_finish();
                        }
                    }
                    else if (io_init_wait_int_low_to < 5) {
                        /* if apollo not up in the last gap, we can wait more gap. */
                        APOLLO_DBG_LOG("init wait int low timeout");
                        wait_for_int_low(40);
                        io_init_wait_int_low_to ++;
                    }
                    else {
                        /* apollo finally can not go up, marked as error. */
                        apollo_state = APOLLO_STATE_ERR;
                        IO_LOW(APOLLO_OVERRIDE_IO);
                        apollo_init_finish();
                    }
                }else if(APOLLO_STATE_START_UP == apollo_state)
                {
                    if (io_init_wait_int_low_interrupt < 5) {
                        /* if apollo not up in the last gap, we can wait more gap. */
                        APOLLO_DBG_LOG("init wait int low interrupt timeout");
                        wait_for_int_low(40);
                        io_init_wait_int_low_interrupt ++;
                    }
                    else {
                        /* apollo finally can not go up, marked as error. */
                        apollo_state = APOLLO_STATE_ERR;
                        IO_LOW(APOLLO_OVERRIDE_IO);
                        apollo_init_finish();
                    }
                }
            }
            else if(APOLLO_CMD_WAIT_TIMEOUT == message->command) {
                if (APOLLO_STATE_INIT_IO == apollo_state) {
                    apollo_state = APOLLO_STATE_START_UP;
                    IO_HIGH(APOLLO_RESET_IO);
                    wait_for_int_low(60);
                }
            }
            break;
        }
        default:
            break;
    }
}

static int send_successful_upgrade(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    int ret = apollo_send_cmd(APOLLO_RESET_2, handle);
    hwi2cClose(handle);
    return ret;
}

/* apollo upgrade handler */
static void apollo_upgrade_handler(MessageId id, Message msg)
{
    switch (id) {
        case MESSAGE_PIO_CHANGED:
        {
            if (0 == GET_INT) {
                switch (apollo_state) {
                    case APOLLO_STATE_UPGRADE_S1: {
                        int trans_bytes_calc = upgrade_remain_bytes;
                        int remain_bytes = transfer_image();
                        trans_bytes_calc -= remain_bytes;
                        APOLLO_DBG_LOG("transfer bytes: %d, remain: %d", trans_bytes_calc, remain_bytes);
                        if (0 == remain_bytes) apollo_state = APOLLO_STATE_UPGRADE_S2;
                        break;
                    }
                    case APOLLO_STATE_UPGRADE_S2: {
                        int ret = transfer_image_3();
                        release_image();
                        if (!ret) {
                            APOLLO_DBG_LOG("upgrade success, reboot.");
                            apollo_state = APOLLO_STATE_UPGRADE_S3;
                            send_successful_upgrade();
                            break;
                        }
                        else if (upgrade_times < 3) {
                            upgrade_times ++;
                            APOLLO_DBG_LOG("upgrade fail, re-init and try upgrade again");
                        } else {
                            apollo_init_end_cb = NULL;
                            APOLLO_DBG_LOG("upgrade fail, re-init and stop upgrade");
                            break;
                        }

                        apollo_state = APOLLO_STATE_INIT_IO;
                        wait_for_timeout(10);
                        break;
                    }
                case APOLLO_STATE_UPGRADE_S3:{
                    uint32 feedback[2];
                    PanicNotZero(apollo_fb((uint8*)feedback, 8));
                    PanicFalse(0x02 == feedback[0]);

                    apollo_state = APOLLO_STATE_UPGRADE_S4;
                    IO_LOW(APOLLO_OVERRIDE_IO);
                    IO_LOW(APOLLO_RESET_IO);
                    wait_for_timeout(10);

                    break;
                }
                    default:
                        break;
                }
            }
        }
        case APOLLO_MESSAGE_CMD:
        {
            APOLLO_COMMAND_T *message = (APOLLO_COMMAND_T*)msg;

            switch (message->command) {
                case APOLLO_CMD_WAIT_INT_LOW: {
                    if (APOLLO_STATE_ENTERING_BOOT_MODE_S1 == apollo_state) {
                        if (INT_IS_LOW) {
                            /* we are entering boot mode in IO driving method, so we are assumed
                             * to success by default.
                             */
                            IO_LOW(APOLLO_OVERRIDE_IO);
                            apollo_state = APOLLO_STATE_IDLE;
                            if (apollo_enter_boot_cb) apollo_enter_boot_cb();
                        } else {
                            if (enter_boot_int_times < 5) {
                                enter_boot_int_times++;
                                APOLLO_DBG_LOG("enter boot mode wait int low timeout");
                                wait_for_int_low(50);
                            } else {
                                APOLLO_DBG_LOG("enter boot mode fail");
                                apollo_state = APOLLO_STATE_ERR;
                                IO_LOW(APOLLO_OVERRIDE_IO);
                            }
                        }
                    }
                    break;
                }
                case APOLLO_CMD_WAIT_TIMEOUT: {
                    if (APOLLO_STATE_ENTERING_BOOT_MODE_S1 == apollo_state) {
                        IO_HIGH(APOLLO_RESET_IO);
                        wait_for_int_low(60);
                    }
                    if (APOLLO_STATE_UPGRADE_S4 == apollo_state) {
                        apollo_state = APOLLO_STATE_START_UP;
                        io_init_wait_int_low_to = 0;
                        io_init_i2c_rd_err_times = 0;
                        io_init_rd_fw_err_times = 0;
                        enter_boot_int_times = 0;
                        upgrade_times = 0;
                        io_init_wait_int_low_interrupt = 0;
                        apollo_init_end_cb = NULL;
                        IO_HIGH(APOLLO_RESET_IO);
                        wait_for_int_low(60);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

static void apollo_common_handler(MessageId id, Message msg)
{
    switch (id) {
        case MESSAGE_PIO_CHANGED:
        {
            if (0 == GET_INT) {
                switch (apollo_state) {
                    case APOLLO_STATE_IDLE: {
                        uint32 feedback[2];
                        PanicNotZero(apollo_fb((uint8*)feedback, 8));
                        if (APOLLO_ACK_WAKEUP == feedback[0]) {
                            APOLLO_DBG_LOG("apollo wakeup.");
                            if (apollo_wakeup_cb) apollo_wakeup_cb();
                        }
                        break;
                    }
                    case APOLLO_STATE_READING_FW_VER: {
                        uint32 feedback[2];
                        PanicNotZero(apollo_fb((uint8*)feedback, 8));

                        if (APOLLO_ACK_RD_FW_VER == feedback[0]) {
                            uint8 *fb = (uint8*)feedback;
                            APOLLO_DBG_LOG8("fw ver", fb);
                            apollo_fw_ver = feedback[1];
                            apollo_state = APOLLO_STATE_IDLE;
                        }
                        else if (APOLLO_ACK_WAKEUP == feedback[0]) {
                            APOLLO_DBG_LOG("wakup, read fw again...");
                            if (apollo_wakeup_cb) apollo_wakeup_cb();
                            read_fw_version();
                        }

                        break;
                    }
                    case APOLLO_STATE_ENTERING_SLEEP: {
                        uint32 feedback[2];
                        int ret = apollo_fb((uint8*)feedback, 8);

                        if (!ret) {
                            APOLLO_DBG_LOG("get other int, try enter sleep again.");
                            bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
                            if (0 == apollo_send_cmd(APOLLO_SLEEP, handle)){
                                /* wait for another int. */
                                hwi2cClose(handle);
                                return;
                            }
                            hwi2cClose(handle);
                        }

                        APOLLO_DBG_LOG("enter sleep.");
                        apollo_state = APOLLO_STATE_SLEEP;

                        InputEventManagerUnregisterTask(apolloTask, APOLLO_INT_IO);

                        uint32 bank = PIO2BANK(APOLLO_INT_IO);
                        uint32 mask = PIO2MASK(APOLLO_INT_IO);
                        PioSetMapPins32Bank(bank, mask, mask);
                        PioSetDir32Bank(bank, mask, mask);
                        IO_LOW(APOLLO_INT_IO);

                        break;
                    }
                    default:
                        break;
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
                case APOLLO_CMD_WAIT_TIMEOUT: {
                    if (APOLLO_STATE_AWAKING == apollo_state) {
                        APOLLO_DBG_LOG("exit sleep");
                        apollo_state = APOLLO_STATE_IDLE;
                        uint32 bank = PIO2BANK(APOLLO_INT_IO);
                        uint32 mask = PIO2MASK(APOLLO_INT_IO);
                        PioSetMapPins32Bank(bank, mask, mask);
                        PioSetDir32Bank(bank, mask, 0);
                        InputEventManagerRegisterTask(apolloTask, APOLLO_INT_IO);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
}

static void apollo_init_finish(void) {
    MessageSend(appGetAppTask(), APOLLO_INIT_CFM, NULL);
}


static bool should_upgrade(void) {
    const apollo_image_header_t *header = (const apollo_image_header_t*) map_image();
    apollo_up_img_ver = header->version;
    release_image();
    return (apollo_fw_ver < apollo_up_img_ver);
}

static int apollo_check_and_start_upgrade(void) {
    int ret = -1;

    if (RUNNING_IN_APP == check_fw_ver(apollo_fw_ver)) {
        if (should_upgrade())
            ret = apollo_start_boot_mode();
        else
            apollo_state = APOLLO_STATE_IDLE;
        APOLLO_DBG_LOG("upgrade img ver: 0x%x", apollo_up_img_ver);
    }
    else if (RUNNING_IN_BOOT == check_fw_ver(apollo_fw_ver)) {
        APOLLO_DBG_LOG("in boot mode, start upgrading direct.");
        apollo_start_new_image();
    }

    return ret;
}

static void start_boot_mode_1(void) {
    /* drive override high during boot up to enter boot mode. */
    IO_HIGH(APOLLO_OVERRIDE_IO);
    IO_LOW(APOLLO_RESET_IO);
    /* keep reset low for 10ms */
    wait_for_timeout(10);
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
    apollo_send_cmd(APOLLO_START_UPGRADE, handle);

    hwi2cClose(handle);

    return 0;
}

static int check_fw_ver(uint32 fw_ver) {
    /* most significant byte of boot fw version is 0xFF */
    if (-1 == fw_ver)
        return RUNNING_FW_NV;
    else if (0xFF == (fw_ver >> 24))
        return RUNNING_IN_BOOT;
    else
        return RUNNING_IN_APP;
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

    hwi2cClose(handle);

    return (3 == ret) ? 0 : -1;
}


/* apollo task handler */
static void apollo_task_handler(Task appTask, MessageId id, Message msg)
{
    UNUSED(appTask);
    if (apollo_state < APOLLO_STATE_INIT_END)
        apollo_init_handler(id, msg);
    else if (apollo_state < APOLLO_STATE_UPGRADE_END)
        apollo_upgrade_handler(id, msg);
    else
        apollo_common_handler(id, msg);
}

static int read_fw_version(void)
{
    bitserial_handle handle = PanicZero(hwi2cOpen(APOLLO_CHIPADDR, APOLLO_I2C_FREQ));
    int ret = apollo_send_cmd(APOLLO_GET_FW_VER, handle);
    hwi2cClose(handle);
    return ret;
}

/*
 * general utilities
*/
static void wait_for_int_low(int delay) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = APOLLO_CMD_WAIT_INT_LOW;
    MessageSendLater(apolloTask, APOLLO_MESSAGE_CMD, message, delay);
}

static void wait_for_timeout(int delay) {
    MAKE_APOLLO_MESSAGE(APOLLO_COMMAND);
    message->command = APOLLO_CMD_WAIT_TIMEOUT;
    MessageSendLater(apolloTask, APOLLO_MESSAGE_CMD, message, delay);
}

static int apollo_send_cmd(uint32 cmd, bitserial_handle handle)
{
    int ret = -1;
    if (0 != handle) {
        uint32 cmd_buf_w[2];
        uint8 *cmd_buf = (uint8*)cmd_buf_w;

        cmd_buf[3] = APOLLO_COMMAND;
        cmd_buf_w[1] = cmd;

        ret = hwi2cWrite(handle, cmd_buf + 3, 5);
    }
    return ret;
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
    /*DO NOT FORGET to release file image*/

    const char apollo_image_name[] = UPGRADE_IMG_FILE_NAME;

    FILE_INDEX idx = FileFind(FILE_ROOT, apollo_image_name, strlen(apollo_image_name));

    return (FILE_NONE == idx) ? NULL : FileMap(idx, 0, FILE_MAP_SIZE_ALL);
}

static void release_image(void) {
    FileUnmap(apollo_img_header);
}
