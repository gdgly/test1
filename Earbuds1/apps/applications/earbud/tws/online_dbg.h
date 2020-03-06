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

/*
 * "data": [
 *      {
 *          "type":"0x01",
 *          "function":"apollo init success"
 *      },
 *      {
 *          "type":"0x02",
 *          "function":"apollo init fail"
 *      },
 *      {
 *          "type":"0x03",
 *          "function":"plc init success"
 *      },
 *      {
 *          "type":"0x04",
 *          "function":"plc init fail"
 *      },
 *      {
 *          "type":"0x05",
 *          "function":"proximity init success"
 *      },
 *      {
 *          "type":"0x06",
 *          "function":"proximity init fail"
 *      },
 *      {
 *          "type":"0x07",
 *          "function":"lisdw12 init success"
 *      },
 *      {
 *          "type":"0x08",
 *          "function":"lisdw12 init fail"
 *      },
 *      {
 *          "type":"0x09",
 *          "function":"in case"
 *      },
 *      {
 *          "type":"0x0A",
 *          "function":"out of case"
 *      },
 *      {
 *          "type":"0x0B",
 *          "function":"in ear"
 *      },
 *      {
 *          "type":"0x0C",
 *          "function":"out of ear"
 *      },
 *      {
 *          "type":"0x0D",
 *          "function":"double tap"
 *      },
 *      {
 *          "type":"0x0E",
 *          "function":"invalid double tap"
 *      },
 *      {
 *          "type":"0x0F",
 *          "function":"case open"
 *      },
 *      {
 *          "type":"0x10",
 *          "function":"case close"
 *      },
 *      {
 *          "type":"0x20",
 *          "function":"avdtp discover"
 *      },
 *      {
 *          "type":"0x21",
 *          "function":"avdtp get capabilities"
 *      },
 *      {
 *          "type":"0x22",
 *          "function":"avdtp set configuration"
 *      },
 *      {
 *          "type":"0x23",
 *          "function":"avdtp get configuration"
 *      },
 *      {
 *         "type":"0x24",
 *         "function":"avdtp re-configure"
 *      },
 *      {
 *         "type":"0x25",
 *         "function":"avdtp open"
 *      },
 *      {
 *         "type":"0x26",
 *         "function":"avdtp start"
 *      },
 *      {
 *         "type":"0x27",
 *         "function":"avdtp close"
 *      },
 *      {
 *         "type":"0x28",
 *         "function":"avdtp suspend"
 *      },
 *      {
 *         "type":"0x29",
 *         "function":"avdtp abort"
 *      },
 *      {
 *         "type":"0x2A",
 *         "function":"avdtp security control"
 *      },
 *      {
 *         "type":"0x2B",
 *         "function":"avdtp get all capabilities"
 *      },
 *      {
 *         "type":"0x2C",
 *         "function":"avdtp delay report"
 *      }
 *  ],
 *  "version":"0.01.01"
 * }
 */

#define ONLINE_DBG_APO_INIT_SUCC    (0x01)    /* apollo init success */
#define ONLINE_DBG_APO_INIT_FAIL    (0x02)    /* apollo init fail */
#define ONLINE_DBG_PLC_INIT_SUCC    (0x03)    /* plc init success */
#define ONLINE_DBG_PLC_INIT_FAIL    (0x04)    /* plc init fail */
#define ONLINE_DBG_PROX_INIT_SUCC   (0x05)    /* proximity init success */
#define ONLINE_DBG_PROX_INIT_FAIL   (0x06)    /* proximity init fail */
#define ONLINE_DBG_TAP_INIT_SUCC    (0x07)    /* lisdw12 init success */
#define ONLINE_DBG_TAP_INIT_FAIL    (0x08)    /* lisdw12 init fail */
#define ONLINE_DBG_IN_CASE          (0x09)    /* in case */
#define ONLINE_DBG_OUT_CASE         (0x0A)    /* out of case */
#define ONLINE_DBG_IN_EAR           (0x0B)    /* in ear */
#define ONLINE_DBG_OUT_EAR          (0x0C)    /* out of ear */
#define ONLINE_DBG_DOUBLE_TAP       (0x0D)    /* double tap */
#define ONLINE_DBG_DOUBLE_TAP_INV   (0x0E)    /* invalid double tap */
#define ONLINE_DBG_CASE_OPEN        (0x0F)    /* case open */
#define ONLINE_DBG_CASE_CLOSE       (0x10)    /* case close */

#define ONLINE_DBG_AVDTP_DISCOVER   (0x20)    /* avdtp discover */
#define ONLINE_DBG_AVDTP_GET_CAPS   (0x21)    /* avdtp get capabilities */
#define ONLINE_DBG_AVDTP_SET_CONFIG (0x22)    /* avdtp set configuration */
#define ONLINE_DBG_AVDTP_GET_CONFIG (0x23)    /* avdtp get configuration */
#define ONLINE_DBG_AVDTP_RE_CONFIG  (0x24)    /* avdtp re-configure */
#define ONLINE_DBG_AVDTP_OPEN       (0x25)    /* avdtp open */
#define ONLINE_DBG_AVDTP_START      (0x26)    /* avdtp start */
#define ONLINE_DBG_AVDTP_CLOSE      (0x27)    /* avdtp close */
#define ONLINE_DBG_AVDTP_SUSPEND    (0x28)    /* avdtp suspend */
#define ONLINE_DBG_AVDTP_ABORT      (0x29)    /* avdtp abort */
#define ONLINE_DBG_AVDTP_SEC_CTRL   (0x2A)    /* avdtp security control */
#define ONLINE_DBG_AVDTP_GET_ALLCAP (0x2B)    /* avdtp get all capabilities */
#define ONLINE_DBG_AVDTP_DEL_REP    (0x2C)    /* avdtp delay report */


typedef enum {
    ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_START   = 0x01,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_START   = 0x01,    /* dev response packet format */
    ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_STOP    = 0x02,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_STOP    = 0x02,    /* dev response packet format */
    ONLINE_DBG_APP_REQ_SYSTEM_STATUS         = 0x03,    /* app request packet format */
    ONLINE_DBG_DEV_RSP_SYSTEM_STATUS         = 0x03,    /* dev response packet format */
    ONLINE_DBG_DEV_UPLOAD_RT_ONLINE_DBG      = 0x04,    /* dev send packet format */
    ONLINE_DBG_DEV_UPLOAD_SYSTEM_STATUS      = 0x05,    /* dev send packet format */
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
