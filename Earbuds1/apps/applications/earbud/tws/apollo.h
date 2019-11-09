#ifndef APOLLO_H
#define APOLLO_H

#define APOLLO_CHIPADDR         (0x10) // (0x68)
#define APOLLO_I2C_FREQ         (400)

#define APOLLO_INT_IO           (0x06)

#define APOLLO_GET_SW_VER       (0x06) // get software version

#define APOLLO_FEEDBACK         (0x00)
#define APOLLO_COMMAND          (0x80)
#define APOLLO_DATA             (0x84)

#define APOLLO_UPGRADE_MESSAGE_BASE (0x11)

void apollo_int_io_init(void);
void apollo_init(void);
void apollo_start_boot_mode(void);
void apollo_start_new_image(void);
void apollo_transfer_image(void);
void apollo_read_fw_version(void);
void apollo_reset(void);

int apolloGetStatus(void);

typedef struct
{
    TaskData data;
    Source   data_source_sco;
    Source   data_source_mic;
#ifndef GAIA_TEST
    unsigned int msg_cnt_sco;
    unsigned int msg_cnt_mic;
#endif
} ApolloTaskData;

typedef struct {
    uint32 command;
} APOLLO_COMMAND_T;

typedef enum apollo_upgrade_state {
    APOLLO_UPGRADE_STAGE1 = APOLLO_UPGRADE_MESSAGE_BASE,
    APOLLO_UPGRADE_STAGE2,
    APOLLO_UPGRADE_CMD,
    APOLLO_MESSAGE_CMD
} apollo_upgrade_state_t;

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

#endif // APOLLO_H
