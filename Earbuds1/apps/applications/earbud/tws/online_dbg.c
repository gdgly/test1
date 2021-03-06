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

/* 写入日志文件,调试使用 */
#define WRITE2LOGFILE  0
/*
 * 写日志到文件里
 * @code:状态码
 */
uint8 sign = 0;
static void wirteLog2File(online_dbg_t code)
{
    rtime_t timeStamp = 0;
    uint16 fileLogIndex = 0;
    static int16 fileSize;
    uint8 buffer[5];

    /* 写入日志 */
    if (appInitCompleted())
    {
        fileLogIndex = FindFileIndex(FILE_LOG);
        if (fileLogIndex == FILE_NONE)
        {
            fileSize = 0;
            fileLogIndex = FileCreate(FILE_LOG, (uint16)strlen(FILE_LOG));
            goto write;
        }
        else
        {
            if (sign == 0)
            {
                sign = 1;
                /* 频繁获取文件大小会系统崩溃，所以只在启动获取一次 */
                fileSize = getFileSize(fileLogIndex);
            }
            else
                fileSize += sizeof (buffer)/sizeof (uint8);
            if (fileSize < MAX_LOG_SIZE && fileSize > 0)
            {
                goto write;
            }
            return;
        }
    write:
        /* 写入时间戳和code日志 */
        timeStamp = SystemClockGetTimerTime();
        for (int i = 0; i < 4; i++)
        {
            buffer[i] = (timeStamp>>(i*8))&0xFF;
        }
        buffer[4] = code;
        FileWrite(fileLogIndex, buffer, sizeof (buffer)/sizeof (uint8));
        ONLINE_DBG_LOG("timeStame = %u,code = %d",timeStamp,code);
    }
}

#ifdef MAX20340_DEBUG_LOG_ERR_TIMES
extern unsigned short data_err_num_ble;
extern unsigned short data_right_num_ble;
#endif

void online_dbg_record(online_dbg_t code) {
    online_dbg_buf[record_idx++] = code;
#ifdef MAX20340_DEBUG_LOG_ERR_TIMES
    if(code == 0x36){
        online_dbg_buf[record_idx++] = (data_err_num_ble >> 8) & 0xff;
        online_dbg_buf[record_idx++] = (data_err_num_ble) & 0xff;
    }
    if(code == 0x37){
        online_dbg_buf[record_idx++] = (data_right_num_ble >> 8) & 0xff;
        online_dbg_buf[record_idx++] = (data_right_num_ble) & 0xff;
    }
#endif
    if (record_idx == trans_idx) trans_idx++;

    UartPuts2x("onLINE=", code, 0);
    DEBUG_LOG("online dbg record :%02X", code);

    if ((ONLINE_DBG_STATE_RT_PACKET == online_dbg_state)
            && (record_idx - trans_idx > SEND_PKT_LENGTH)) {
        MessageSend(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG, NULL);
    }

    if (WRITE2LOGFILE)
        wirteLog2File(code);

}

void online_dbg_record_sync_20340_reg_value(uint8 self_reg, uint8 other_reg, uint8 itr_status) {
    online_dbg_buf[record_idx++] = ONLINE_DBG_20340_REG_VALUE;
    online_dbg_buf[record_idx++] = self_reg;
    online_dbg_buf[record_idx++] = other_reg;
    online_dbg_buf[record_idx++] = itr_status;
    if (record_idx == trans_idx) trans_idx++;

    UartPuts2x("onLINE=", ONLINE_DBG_20340_REG_VALUE, 0);
    DEBUG_LOG("online dbg record :%02X", ONLINE_DBG_20340_REG_VALUE);

    if ((ONLINE_DBG_STATE_RT_PACKET == online_dbg_state)
            && (record_idx - trans_idx > SEND_PKT_LENGTH)) {
        MessageSend(onlineDbgTask, ONLINE_DBG_MSG_TRANS_RT_ONLINE_DBG, NULL);
    }

    if (WRITE2LOGFILE)
        wirteLog2File(ONLINE_DBG_20340_REG_VALUE);
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

//    if(cmd == 0x36 || cmd == 0x37){
//        if(cmd == 0x36){
//            packet.payload[10] = (data_err_num_ble >> 8) & 0xff;
//            packet.payload[11] = (data_err_num_ble >> 8);
//        }else{
//            packet.payload[10] = (data_right_num_ble >> 8) & 0xff;
//            packet.payload[11] = (data_right_num_ble >> 8);
//        }
//        packet.payload_len = length + 2;
//        appGaiaSendPacket(GAIA_VENDOR_STAROT,
//                          GAIA_COMMAND_STAROT_TEST_ONLINE_DBG, 0xfe, sizeof(packet), (uint8*)&packet);
//    }else{
//        appGaiaSendPacket(GAIA_VENDOR_STAROT,
//                          GAIA_COMMAND_STAROT_TEST_ONLINE_DBG, 0xfe, sizeof(packet) - 2, (uint8*)&packet);
//    }
    appGaiaSendPacket(GAIA_VENDOR_STAROT,
                      GAIA_COMMAND_STAROT_TEST_ONLINE_DBG, 0xfe, sizeof(packet), (uint8*)&packet);
}

void online_dbg_print(void) {
    for (uint8 i = trans_idx; i != record_idx; ++i) {
        DEBUG_LOG("print dbg record : %02X", online_dbg_buf[i]);
    }
}
