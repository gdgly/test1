#include <Panic.h>
#include <stdlib.h>
#include <message.h>
#include <string.h>

#include "pio_common.h"

#include "av_headset.h"
#include "host.h"
#include "public.h"

#ifdef CUMMPC_PC_USB

#define WORD_SIZE_BYTES 2
#define HOST_COMMS_HEADER_SIZE_WORDS 2
#define HOST_COMMS_SIZE_OFFSET_WORD 0
#define HOST_COMMS_CHANNEL_OFFSET_WORD 1
#define HOST_COMMS_PAYLOAD_OFFSET_WORD 2

#define COMMU_INTERVAL_TIMER      (2000)
enum { TYPE_GET_VER=0, TYPE_BTADDR, TYPE_LIS25, TYPE_EM20168, TYPE_LIS2DW12, TYPE_MAX20340, TYPE_APOLLO, TYPE_LAST};

typedef struct tagCOMMUINFO {
    TaskData  task;
    char      outbuf[64];
    uint8     outsize;
    uint8_t   type;    // TYPE_XXX
}CommuInfo,*pCommuIPtr;

static CommuInfo gCommInfo;

extern int lis25GetStatus(void);
extern int apolloGetStatus(void);

static void CheckAllItem(CommuInfo *com)
{
    uint8 ver[8], addr[8];
    int status;

    com->outsize = 0;
    memset(com->outbuf, 0, sizeof(com->outbuf));
    switch(com->type){
        case TYPE_GET_VER:                               // 获取当前耳机版本信息
            SystemGetCurrentVersion((uint8*)ver);
            memcpy(com->outbuf, ver, sizeof(ver));
            com->outsize = sprintf(com->outbuf, "check %sVERSION %02X.%02X.%02X-%02X.%02X.%02X.%02X",
                    appConfigIsLeft()? "RIGHT" : "LEFT",ver[0], ver[1], ver[2], ver[4], ver[5], ver[6], ver[7]);
            break;
        case TYPE_BTADDR:                           // 获取耳机经典蓝牙地址
            SystemGetEarAddr((uint8 *)addr);
            memcpy(com->outbuf, addr, sizeof(addr));
            com->outsize = sprintf(com->outbuf, "check BTADDR %02X:%02X:%02X:%02X:%02X:%02X",
                    addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
            break;
        case TYPE_LIS25:
            status = lis25GetStatus();
            com->outsize = sprintf(com->outbuf, "check LIS25 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#ifdef HAVE_EM20168
        case TYPE_EM20168:
            status = EM20168_GetStatus();
            com->outsize = sprintf(com->outbuf, "check EM20168 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
#ifdef HAVE_LIS2DW12
        case TYPE_LIS2DW12:
            status = lis2dw12_GetStatus();
            com->outsize = sprintf(com->outbuf, "check LIS2DW12 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
#ifdef HAVE_MAX20340
        case TYPE_MAX20340:
            status = max20340_GetStatus();
            com->outsize = sprintf(com->outbuf, "check MAX20340 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
        case TYPE_APOLLO:
            status = apolloGetStatus();
            com->outsize = sprintf(com->outbuf, "check APOLLO %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
        default:
            return;
    }
}

static void handleTransportSendData(Task task, uint8* message, uint16 messageSize)
{
    uint8 * byte_data;
    uint16 * word_data;
    uint16 host_msg_size_in_words = HOST_COMMS_HEADER_SIZE_WORDS;

    (void)task;
    host_msg_size_in_words += messageSize / sizeof(uint16) + messageSize % sizeof(uint16);
    /* Transfer the message payload to a new Host Comms message and pass toHostSendMessage. */
    byte_data = PanicUnlessMalloc(host_msg_size_in_words * sizeof(uint16));
    word_data = (uint16 *)byte_data;
    word_data[HOST_COMMS_SIZE_OFFSET_WORD] = host_msg_size_in_words;
    word_data[HOST_COMMS_CHANNEL_OFFSET_WORD] = 0;
    memmove(&byte_data[HOST_COMMS_PAYLOAD_OFFSET_WORD * sizeof(uint16)], message, messageSize);

    HostSendMessage(word_data);
}

/* Task handler function */
static void CummuHandler(Task task, MessageId id, Message message)
{
    CommuInfo *pComu = (CommuInfo*)task;
    switch (id)
    {
        case MESSAGE_FROM_HOST:
        {
            uint8* msg = (uint8 *)message;
            uint8* payload = &msg[HOST_COMMS_PAYLOAD_OFFSET_WORD * sizeof(uint16)];
            //uint16 size = msg[HOST_COMMS_SIZE_OFFSET_WORD * sizeof(uint16)];

            /* Handle the message from the host here*/
            if(strstr((char *)payload, "check DEVICEINFO")){
                pComu->type = 0;
                MessageCancelAll(task, COMMU_INTERVAL_TIMER);
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 1);
            }
            break;
        }

        case COMMU_INTERVAL_TIMER:
        {
            pComu->outsize = 0;
            CheckAllItem(pComu);
            if(pComu->outsize > 0) {
                handleTransportSendData(task, (uint8 *)pComu->outbuf, pComu->outsize);                
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 10);
            }
            if(pComu->type < TYPE_LAST)
                pComu->type  += 1;
            break;
        }
    }
}


void CummuInit(void)
{
    DEBUG_LOG("Test CONNECTION started");

    memset(&gCommInfo, 0, sizeof(gCommInfo));
    gCommInfo.task.handler = CummuHandler;    /* Set up task handler */

    /* Register a task to receive messages over host comms */
    if (MessageHostCommsTask(&gCommInfo.task)) {
        DEBUG_LOG("Error in host comms task registration");
        Panic();
    }
}
#endif
