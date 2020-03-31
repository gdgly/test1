#include <string.h>
#include <panic.h>
#include <message.h>
#include <stdio.h>
#include <vmal.h>
#include <stdlib.h>
#include "logging.h"
#include "online_dbg.h"
#include "av_headset_gaia_starot.h"
#include "gaia.h"
#include <system_clock.h>
#include "rwfile.h"

#define ENABLE_DBG_PRINT    (1)

#if (ENABLE_DBG_PRINT == 1)
#define ONLINE_DBG_LOG(...) DEBUG_LOG("online_dbg: " __VA_ARGS__)
#else
#define ONLINE_DBG_LOG(...)
#endif

#ifdef GAIA_TEST
bool appGaiaSendPacket(uint16 vendor_id, uint16 command_id,
                       uint16 status, uint16 payload_length, uint8 *payload);
#else
void appGaiaSendPacket(uint16 vendor_id, uint16 command_id,
                       uint16 status, uint16 payload_length, uint8 *payload);
#endif

#define min(a, b)   ((a) < (b) ? (a) : (b))

#define MAKE_ONLINE_DBG_MESSAGE(TYPE) TYPE##_T *message = PanicUnlessNew(TYPE##_T)
#define ONLINE_DBG_BUF_LEN    (256)

static OnlineDbgTaskData onlineDbgTaskData = {.data = {.handler = online_dbg_handler}};
static Task onlineDbgTask = &(onlineDbgTaskData.data);

static online_dbg_t online_dbg_buf[ONLINE_DBG_BUF_LEN];
static volatile uint8 online_dbg_state = ONLINE_DBG_STATE_IDLE;
static volatile uint8 record_idx = 0;
static volatile uint8 trans_idx = 0;

static void online_dbg_resp(uint8 cmd, uint8 status);
static void online_dbg_send_pkt(online_dbg_cmd cmd, uint8* data, uint16 length);

uint8 get_online_dbg_state(void) {
    DEBUG_LOG("online_dbg state: %d", online_dbg_state);
    return online_dbg_state;
}

void online_dbg_record(online_dbg_t code) {
    uint16 fileLogIndex = 0;
    rtime_t timeStamp = 0;
    int16 fileSize = 0;

    online_dbg_buf[record_idx++] = code;
    if (record_idx == trans_idx) trans_idx++;

    if ((ONLINE_DBG_STATE_RT_PACKET == online_dbg_state)
            && (record_idx - trans_idx > SEND_PKT_LENGTH)) {
        MessageSend(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG, NULL);
    }
    /* 写入日志 */
    fileLogIndex = FindFileIndex(FILE_LOG);
    if (fileLogIndex == FILE_NONE)
    {
        fileLogIndex = FileCreate(FILE_LOG, (uint16)strlen(FILE_LOG));
        goto write;
    }
    else
    {
        fileSize = getFileSize(fileLogIndex);
        if (fileSize < MAX_LOG_SIZE && fileSize >= 0)
        {
            goto write;
        }
        return;
    }
write:
    /* 写入时间戳 */
    timeStamp = SystemClockGetTimerTime();
    FileWrite(fileLogIndex,(uint8 *)(&timeStamp),sizeof (rtime_t));
    /* 写入状态码 */
    FileWrite(fileLogIndex,(uint8 *)&code,sizeof (online_dbg_t));
    ONLINE_DBG_LOG("timeStame = %u,code = %d",timeStamp,code);
}

void online_dbg_cmd_handler(online_dbg_cmd cmd) {
    if ((ONLINE_DBG_STATE_IDLE == online_dbg_state)
            || ((ONLINE_DBG_STATE_RT_PACKET == online_dbg_state) && (ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_STOP == cmd))) {
        switch(cmd) {
            case ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_START:
                ONLINE_DBG_LOG("ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_START");
                online_dbg_state = ONLINE_DBG_STATE_RT_PACKET;
                online_dbg_resp(ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_START, RESP_SUCC);
                MessageSend(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG, NULL);
                MessageSendLater(onlineDbgTask, ONLINE_DBG_MSG_STOP_RT_ONLINE_DBG, NULL, 1000 * 3600 * 10);
                break;
            case ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_STOP:
                ONLINE_DBG_LOG("ONLINE_DBG_APP_REQ_RT_ONLINE_DBG_STOP");
                online_dbg_state = ONLINE_DBG_STATE_IDLE;
                online_dbg_resp(ONLINE_DBG_DEV_RSP_RT_ONLINE_DBG_STOP, RESP_SUCC);
                MessageCancelAll(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG);
                break;
            case ONLINE_DBG_APP_REQ_SYSTEM_STATUS:
                ONLINE_DBG_LOG("ONLINE_DBG_APP_REQ_SYSTEM_STATUS");
                online_dbg_state = ONLINE_DBG_STATE_SYS_STATUS;
                online_dbg_resp(ONLINE_DBG_DEV_RSP_SYSTEM_STATUS, RESP_SUCC);
                MessageSend(onlineDbgTask, ONLINE_DBG_MSG_TRANS_SYS_STATUS, NULL);
                break;
            default:
                ONLINE_DBG_LOG("incorrect online_dbg command: %d", cmd);
                break;
        }
    } else {
        ONLINE_DBG_LOG("online_dbg busy: %d", online_dbg_state);
        online_dbg_resp(cmd, RESP_FAIL);
    }
}

void online_dbg_handler(Task appTask, MessageId id, Message msg) {
    UNUSED(appTask);
    UNUSED(msg);

    switch(id) {
        case ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG:
            if (ONLINE_DBG_STATE_RT_PACKET == online_dbg_state) {
                uint8 remain = record_idx - trans_idx;
                uint8 length = min(min(ONLINE_DBG_BUF_LEN - trans_idx, remain), SEND_PKT_LENGTH);

                if (length > 0) {
                    online_dbg_send_pkt(ONLINE_DBG_DEV_UPLOAD_RT_ONLINE_DBG, online_dbg_buf + trans_idx, length);
                    trans_idx += length;
                }

                MessageSendLater(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG, NULL, 100);
            }
            break;
        case ONLINE_DBG_MSG_STOP_RT_ONLINE_DBG:
            if ((ONLINE_DBG_STATE_RT_PACKET == online_dbg_state)){
                MessageCancelAll(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG);
                MessageCancelAll(onlineDbgTask, ONLINE_DBG_MSG_STOP_RT_ONLINE_DBG);
                online_dbg_state = ONLINE_DBG_STATE_IDLE;
                DEBUG_LOG("online debug real time debug stop");
            }
            break;
        case ONLINE_DBG_MSG_TRANS_SYS_STATUS:
            break;
        default:
            break;
    }
}

static void online_dbg_resp(uint8 cmd, uint8 status) {
    ONLINE_DBG_RESP_T resp = {
        .online_dbg_cmd = cmd,
        .status = status
    };

    appGaiaSendPacket(GAIA_VENDOR_STAROT,
                      GAIA_COMMAND_STAROT_TEST_ONLINE_DBG, 0xfe, sizeof(resp), (uint8*)&resp);
}

static void online_dbg_send_pkt(online_dbg_cmd cmd, uint8* data, uint16 length) {
    ONLINE_DBG_DATA_PKT_T packet = {
        .online_dbg_cmd = cmd,
        .payload_len = length,
    };

    memcpy(packet.payload, data, length);

    appGaiaSendPacket(GAIA_VENDOR_STAROT,
                      GAIA_COMMAND_STAROT_TEST_ONLINE_DBG, 0xfe, sizeof(packet), (uint8*)&packet);
}
