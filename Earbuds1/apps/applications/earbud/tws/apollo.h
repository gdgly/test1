#ifndef APOLLO_H
#define APOLLO_H

#define APOLLO_CHIPADDR         (0x10) // (0x68)
#define APOLLO_I2C_FREQ         (400)

#define APOLLO_INT_IO           (0x06)

#define APOLLO_GET_SW_VER       (0x06) // get software version

#define APOLLO_FEEDBACK         (0x00)
#define APOLLO_COMMAND          (0x80)
#define APOLLO_DATA             (0x84)

void apollo_int_io_init(void);
int apollo_start_boot_mode(void);
void apollo_start_new_image(void);
void apollo_transfer_image(void);
int apollo_read_fw_version(void);
void apollo_reset(void);

int apolloGetStatus(void);

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

/*
 * apollo init state definition
*/
#define    APOLLO_STATE_INIT_IO                     (0)
#define    APOLLO_STATE_INIT_RD_FW_VER              (1)
#define    APOLLO_STATE_INIT_END                    (2)
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



#define APOLLO_UPGRADE_MESSAGE_BASE (0x11)
enum APOLLO_MESSAGE {
    APOLLO_MESSAGE_CMD = APOLLO_UPGRADE_MESSAGE_BASE,
};

/*
 * apollo task command definition
*/
#define APOLLO_CMD_WAIT_INT_LOW (0)
#define APOLLO_CMD_READ_FW_VER  (1)
#define APOLLO_CMD_RESET        (2)

#endif // APOLLO_H
