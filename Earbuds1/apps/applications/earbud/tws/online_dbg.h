#ifndef ONLINE_DBG_H
#define ONLINE_DBG_H

#include <message.h>

/*****************************************************************
 *
 * app request/command packet format:
 * ----------------------------------------------------
 * | byte 0                            | byte 1       |
 * | GAIA_COMMAND_STAROT_TEST_ONLINE_DBG | online_dbg_cmd |
 * ----------------------------------------------------
 *
 * dev response packet format :
 * ----------------------------------------------------------------------
 * | byte 0                            | byte 1       | byte 2          |
 * | GAIA_COMMAND_STAROT_TEST_ONLINE_DBG | online_dbg_cmd | resp status     |
 * ----------------------------------------------------------------------
 * resp status:
 *      0: success; -1: fail
 *
 * dev send packet format:
 * ------------------------------------------------------------------------------------
 * | byte 0                            | byte 1       | byte 2         |      ...     |
 * | GAIA_COMMAND_STAROT_TEST_ONLINE_DBG | online_dbg_cmd | payload length | payload data |
 * ------------------------------------------------------------------------------------
 *
 *******************************************************************/

typedef enum {
    ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_START   = 0x01,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_START   = 0x01,    /* dev response packet format */
    ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_STOP    = 0x02,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_STOP    = 0x02,    /* dev response packet format */
    ONLINE_DBG_APP_REQ_SYSTEM_STATUS       = 0x03,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_SYSTEM_STATUS       = 0x03,    /* dev response packet format */
    ONLINE_DBG_DEV_UPLOAD_RT_ONLINE_DBG      = 0x04,    /* dev send packet format */
    ONLINE_DBG_DEV_UPLOAD_SYSTEM_STATUS    = 0x05,    /* dev send packet format */
} online_dbg_cmd;

typedef uint8 online_dbg_t;

#define SEND_PKT_LENGTH (10)

#define RESP_SUCC       (0)
#define RESP_FAIL       (-1)

#define ONLINE_DBG_STATE_IDLE                 (0x01)
#define ONLINE_DBG_STATE_RT_PACKET            (0x02)
#define ONLINE_DBG_STATE_SYS_STATUS           (0x03)

typedef struct {
    TaskData data;
} OnlineDbgTaskData;

typedef struct {
    uint8 cmd;
} ONLINE_DBG_MSG_T;

typedef struct {
    uint8 online_dbg_cmd;
    uint8 status;
} ONLINE_DBG_RESP_T;

typedef struct {
    uint8  online_dbg_cmd;
    uint16 payload_len;
    uint8  payload[SEND_PKT_LENGTH];
} ONLINE_DBG_DATA_PKT_T;

enum ONLINE_DBG_MESSAGE {
    ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG,
    ONLINE_DBG_MSG_STOP_RT_ONLINE_DBG,
    ONLINE_DBG_MSG_TRANS_SYS_STATUS
};

void online_dbg_record(online_dbg_t code);
uint8 get_online_dbg_state(void);
void online_dbg_handler(Task appTask, MessageId id, Message msg);
void online_dbg_cmd_handler(online_dbg_cmd cmd);

#endif // ONLINE_DBG_H
