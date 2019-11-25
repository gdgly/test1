#include <Panic.h>
#include <stdlib.h>
#include <message.h>
#include <string.h>
#include <source.h>

#include "pio_common.h"

#include "av_headset.h"
#include "host.h"
#include "public.h"
#include "av_headset_gaia_starot.h"
#include "audio_forward.h"

#ifdef CUMMPC_PC_USB

#define WORD_SIZE_BYTES 2
#define HOST_COMMS_HEADER_SIZE_WORDS 2
#define HOST_COMMS_SIZE_OFFSET_WORD 0
#define HOST_COMMS_CHANNEL_OFFSET_WORD 1
#define HOST_COMMS_PAYLOAD_OFFSET_WORD 2

#define COMMU_INTERVAL_TIMER      (2000)
#define COMMU_INTERVAL_RECORD     (2001)
enum { TYPE_GET_VER, TYPE_BTADDR, TYPE_LIS25, TYPE_EM20168, TYPE_LIS2DW12, TYPE_MAX20340, TYPE_APOLLO, TYPE_LAST};


typedef struct tagCOMMUINFO {
    TaskData  task;
    uint8_t   type;    // TYPE_XXX
}CommuInfo,*CommuIPtr;
void CummuhandleSendData(Task task, uint8* message, uint16 messageSize);

CommuInfo gCommInfo;


void CommpcParse(GAIA_STAROT_AUDIO_IND_T *message);

extern int lis25GetStatus(void);
extern int apolloGetStatus(void);
extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);
extern void disable_audio_forward(bool disable);

static void CommuForwardSetDataTime(void)
{
    forwardSetDataClient(DATA_CLIENT_COMMUPC);
}

static int CummuCheckAllItem(CommuInfo *com, char *outbuf, int bufize)
{
    uint8 ver[8], addr[8];
    int status, outsize = 0;

    memset(outbuf, 0,  bufize);
    switch(com->type){
        case TYPE_GET_VER:                               // 获取当前耳机版本信息
            SystemGetCurrentVersion((uint8*)ver);
            memcpy(outbuf, ver, sizeof(ver));
            outsize = sprintf(outbuf, "check %sVERSION %02X.%02X.%02X-%02X.%02X.%02X.%02X",
                    appConfigIsLeft()? "RIGHT" : "LEFT",ver[0], ver[1], ver[2], ver[4], ver[5], ver[6], ver[7]);
            break;
        case TYPE_BTADDR:                           // 获取耳机经典蓝牙地址
            SystemGetEarAddr((uint8 *)addr);
            memcpy(outbuf, addr, sizeof(addr));
            outsize = sprintf(outbuf, "check BTADDR %02X:%02X:%02X:%02X:%02X:%02X",
                    addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
            break;
        case TYPE_LIS25:
            status = lis25GetStatus();
            outsize = sprintf(outbuf, "check LIS25 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#ifdef HAVE_EM20168
        case TYPE_EM20168:
            status = EM20168_GetStatus();
            outsize = sprintf(outbuf, "check EM20168 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
#ifdef HAVE_LIS2DW12
        case TYPE_LIS2DW12:
            status = lis2dw12_GetStatus();
            outsize = sprintf(outbuf, "check LIS2DW12 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
#ifdef HAVE_MAX20340
        case TYPE_MAX20340:
            status = max20340_GetStatus();
            outsize = sprintf(outbuf, "check MAX20340 %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
#endif
        case TYPE_APOLLO:
            status = apolloGetStatus();
            outsize = sprintf(outbuf, "check APOLLO %s",
                    (status == 0) ? "PASS" : "FAIL");
            break;
        default:
            return 0;
    }

    return outsize;
}

void CummuhandleSendData(Task task, uint8* message, uint16 messageSize)
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

void CommpcParse(GAIA_STAROT_AUDIO_IND_T *message)
{   Task com = GetCommuTask();
    const uint8 *ptr = SourceMap(message->source);
    CummuhandleSendData(com, (uint8 *)ptr, message->len);
    SourceDrop(message->source, message->len);
}

/* Task handler function */
static void CummuHandler(Task task, MessageId id, Message message)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    char outbuf[128];
    CommuInfo *pComu = (CommuInfo*)task;
    int outsize;
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

                CummuhandleSendData(task, (uint8*)"checkresp DEVICEINFO", 21);
            }
            if(strstr((char *)payload, "check ACCEPT")){
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 0);
            }
            if(strstr((char *)payload, "check STARTRECORD0")){
                g_appConfigSocMic1 = 0;
                g_appConfigSocMic2 = NO_MIC;
                progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
                appKymeraRecordStart();
                disable_audio_forward(FALSE);
#endif
                MessageSendLater(task, COMMU_INTERVAL_RECORD, NULL, 10);

                CummuhandleSendData(task, (uint8*)"checkresp STARTRECORD", 22);
            }
            if(strstr((char *)payload, "check STARTRECORD1")){
                g_appConfigSocMic1 = NO_MIC;
                g_appConfigSocMic2 = 1;
                progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
                appKymeraRecordStart();
                disable_audio_forward(FALSE);
#endif
                MessageSendLater(task, COMMU_INTERVAL_RECORD, NULL, 10);

                CummuhandleSendData(task, (uint8*)"checkresp STARTRECORD", 22);
            }
            if(strstr((char *)payload, "check STOPRECORD")){
                progRun->recStat  = 0;
#ifdef CONFIG_REC_ASSISTANT
                disable_audio_forward(TRUE);
                appKymeraRecordStop();
#endif
                CummuhandleSendData(task, (uint8*)"checkresp STOPRECORD", 22);
            }
            break;
        }

        case COMMU_INTERVAL_TIMER:
            outsize = CummuCheckAllItem(pComu, outbuf, sizeof(outbuf));
            if(outsize > 0){
                CummuhandleSendData(task, (uint8 *)outbuf, outsize);
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 0);
            }
            if(pComu->type < TYPE_LAST)
                pComu->type  += 1;
            break;

        case COMMU_INTERVAL_RECORD:
            outsize = 0;
            CommuForwardSetDataTime();
            break;

        case GAIA_STAROT_COMMAND_IND:
            CommpcParse((GAIA_STAROT_AUDIO_IND_T *) message);
            break;
    }
}

Task GetCommuTask(void)
{
    return &gCommInfo.task;
}
void do_rec(void);
void do_rec(void)
{
/*    uint8 message[22] = {0};
    uint16 *ptr = (uint16 *)message;
    ptr[0] = 22;
    ptr[1] = 0;
    memcpy(&message[4],"check STARTRECORD",18);
*/
    MAKE_GAIA_MESSAGE_WITH_LEN(GAIA_STAROT_MESSAGE, GAIA_PAYLOAD_LEN);
    message->command      = MESSAGE_FROM_HOST;
    message->payloadLen   = 23;
    memcpy(message->payload,"check STARTRECORD0",19);
    MessageSend(GetCommuTask(), MESSAGE_FROM_HOST, message);
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
