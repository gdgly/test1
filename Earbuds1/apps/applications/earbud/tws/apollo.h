#ifndef APOLLO_H
#define APOLLO_H

typedef struct {
    TaskData data;
} ApolloTaskData;

typedef struct {
    uint32 command;
} APOLLO_COMMAND_T;

typedef struct {
    uint32 pad1;
    uint32 image_addr;
    uint32 image_len;
    uint32 crc;
    uint32 pad2[4];
    uint32 version;
    uint32 pad3[3];
    uint32 image;
} apollo_image_header_t;

typedef int (*apollo_wakeup_cb_t)(void);
typedef int (*apollo_wakeup_cb_param_t)(uint32 p);

/* io init func called in startup initilization. */
void apollo_int_io_init(void);

/* call this function manually if you want to enter boot mode. */
int apollo_start_boot_mode(void);

/* call this function manually if you want to start an upgrade image transfer.
 * Note: you must call this function in boot mode. */
int apollo_start_new_image(void);

/* read apollo firmware version */
int apollo_read_fw_version(void);

/* register wakeup callback */
void register_apollo_wakeup_cb(apollo_wakeup_cb_t cb);

/* reset apollo */
void apollo_reset(void);

int apolloGetStatus(void);

#define APOLLO_CHIPADDR         (0x10) // (0x68)
#define APOLLO_I2C_FREQ         (400)

#define APOLLO_INT_IO           (0x06)
#define APOLLO_RESET_IO         (0x04)
#define APOLLO_START_UPGRADE    (0x02)
#define APOLLO_TRANS_ONE_PACKET (0x03)

#define APOLLO_GET_FW_VER       (0x06) // get firmware version
#define APOLLO_ERASE_HEADER     (0x10)
#define APOLLO_RESET            (0x09)

#define APOLLO_FEEDBACK         (0x00)
#define APOLLO_COMMAND          (0x80)
#define APOLLO_DATA             (0x84)

/*
 * apollo feedback definition
*/
#define APOLLO_ACK_RD_FW_VER    (APOLLO_GET_FW_VER)
#define APOLLO_ACK_WAKEUP       (0x11)
#define APOLLO_ACK_ERASE_HEADER (APOLLO_ERASE_HEADER)

/*
 * apollo init state definition
*/
#define    APOLLO_STATE_UNINIT                      (0)
#define    APOLLO_STATE_INIT_IO                     (APOLLO_STATE_UNINIT + 1)
#define    APOLLO_STATE_INIT_RD_FW_VER              (APOLLO_STATE_UNINIT + 2)
#define    APOLLO_STATE_INIT_END                    (APOLLO_STATE_UNINIT + 3)
/* state before APOLLO_STATE_INIT_END are all used for initialise */
#define    APOLLO_STATE_ENTERING_BOOT_MODE_S1       (APOLLO_STATE_INIT_END + 1)
#define    APOLLO_STATE_ENTERING_BOOT_MODE_S2       (APOLLO_STATE_INIT_END + 2)
#define    APOLLO_STATE_ENTERING_BOOT_MODE_S3       (APOLLO_STATE_INIT_END + 3)
#define    APOLLO_STATE_UPGRADE_S1                  (APOLLO_STATE_INIT_END + 5)
#define    APOLLO_STATE_UPGRADE_S2                  (APOLLO_STATE_INIT_END + 6)
#define    APOLLO_STATE_UPGRADE_END                 (APOLLO_STATE_INIT_END + 7)
/* state before APOLLO_STATE_UPGRADE_STAR are all used for image upgrade */
#define    APOLLO_STATE_IDLE                        (APOLLO_STATE_UPGRADE_END + 1)
#define    APOLLO_STATE_READING_FW_VER              (APOLLO_STATE_UPGRADE_END + 2)
#define    APOLLO_STATE_ERR                         (APOLLO_STATE_UPGRADE_END + 3)

/*
 * apollo task command definition
*/
#define APOLLO_CMD_WAIT_INT_LOW (0)
#define APOLLO_CMD_READ_FW_VER  (1)
#define APOLLO_CMD_RESET        (2)

/*
 * apollo message definition
*/
#define APOLLO_UPGRADE_MESSAGE_BASE (0x11)

enum APOLLO_MESSAGE {
    APOLLO_MESSAGE_CMD = APOLLO_UPGRADE_MESSAGE_BASE,
};


#endif // APOLLO_H
