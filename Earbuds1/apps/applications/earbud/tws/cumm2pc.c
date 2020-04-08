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
#include "apollo.h"
#include "ucs146e0.h"

#ifdef CUMMPC_PC_USB

#define WORD_SIZE_BYTES 2
#define HOST_COMMS_HEADER_SIZE_WORDS 2
#define HOST_COMMS_SIZE_OFFSET_WORD 0
#define HOST_COMMS_CHANNEL_OFFSET_WORD 1
#define HOST_COMMS_PAYLOAD_OFFSET_WORD 2

#define COMMU_INTERVAL_TIMER      (2000)
#define COMMU_INTERVAL_RECORD     (2001)
enum { TYPE_GET_VER, TYPE_BTADDR, TYPE_LIS25, TYPE_EM20168, TYPE_LIS2DW12, TYPE_MAX20340, TYPE_APOLLO, TYPE_APO_VER, TYPE_UCS146E0, TYPE_LAST};


typedef struct tagCOMMUINFO {
    TaskData  task;
    uint8_t   type;    // TYPE_XXX
}CommuInfo,*CommuIPtr;
void CummuhandleSendData(Task task, uint8* message, uint16 messageSize);

CommuInfo gCommInfo;

#define RINGTONE_STOP  RINGTONE_NOTE(REST, HEMIDEMISEMIQUAVER), RINGTONE_END
static const ringtone_note commupc_tone[] =
{
    RINGTONE_TIMBRE(sine), RINGTONE_DECAY(50),
    RINGTONE_NOTE(A7, SEMIBREVE),
    RINGTONE_NOTE(A1, SEMIBREVE),
    RINGTONE_NOTE(B7, SEMIBREVE),
    RINGTONE_NOTE(A9, SEMIBREVE),
    RINGTONE_NOTE(C9, SEMIBREVE),
    RINGTONE_NOTE(D9, SEMIBREVE),
    RINGTONE_NOTE(E9, SEMIBREVE),
    RINGTONE_NOTE(F9, SEMIBREVE),
    RINGTONE_NOTE(G9, SEMIBREVE),
    RINGTONE_STOP
};
static void CummuPlayTone(void)
{
    appKymeraTonePlay(commupc_tone, TRUE, NULL, 0);
}

void CommpcParse(GAIA_STAROT_AUDIO_IND_T *message);

extern int lis25GetStatus(void);
//extern int apolloGetStatus(void);
//extern void comGetApolloVer(uint8 *arr);
extern void appKymeraRecordStart(void);
extern void appKymeraRecordStop(void);

/* 延时赋值数据消息转发方向 */
static void CommuForwardSetDataClient(void)
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
#ifdef HAVE_UCS146E0
        case TYPE_UCS146E0:
            status = Ucs146e0_GetStatus();
            outsize = sprintf(outbuf, "check UCS146E0 %s",
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
        case TYPE_APO_VER:
            comGetApolloVer((uint8*)ver);
            outsize = sprintf(outbuf, "check APOLLOVERSION %02X.%02X.%02X.%02X",
                    ver[0], ver[1], ver[2], ver[3]);
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
    memset(byte_data, 0, host_msg_size_in_words * sizeof(uint16));
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

extern void appSubUISetMicbias(int set);

//作为判断 1：唤醒,2：接近光,3：PLC,4：敲击是否正常工作,发送消息的标记 5:record
// 默认为0是正常模式，0xFF为测试模式，在测试模式下相关信号不需要发给UI做逻辑控制
uint16 g_commuType = 0;

/* Task handler function */
static void CummuHandler(Task task, MessageId id, Message message)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    char outbuf[128];
    CommuInfo *pComu = (CommuInfo*)task;
    int outsize;
    char *p = NULL;
    unsigned short value = 0;
    switch (id)
    {
        case MESSAGE_FROM_HOST:
        {
            uint8* msg = (uint8 *)message;
            uint8* payload = &msg[HOST_COMMS_PAYLOAD_OFFSET_WORD * sizeof(uint16)];
            //uint16 size = msg[HOST_COMMS_SIZE_OFFSET_WORD * sizeof(uint16)];

            // 接收到测试命令，先打开各外设
            if(g_commuType == 0){
                g_commuType == 0xFF;
                progRun->iPowerSaveMode = POWER_MODE_IN_EAR;
                appUiPowerSaveSync();
            }

            /* Handle the message from the host here*/
            if(strstr((char *)payload, "check DEVICEINFO")){
                pComu->type = 0;
                MessageCancelAll(task, COMMU_INTERVAL_TIMER);
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 1);

                CummuhandleSendData(task, (uint8*)"checkresp DEVICEINFO", 21);
            }

            if(strstr((char *)payload, "check STARTRECORD0")){
#ifdef ENABLE_APOLLO
                apollo_sleep();
#endif
                progRun->apolloWakeup = 0;
                g_commuType        = 5;
                g_appConfigSocMic1 = 0;
                g_appConfigSocMic2 = NO_MIC;
                progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
                appKymeraRecordStart();
#endif
                MessageSendLater(task, COMMU_INTERVAL_RECORD, NULL, 10);

                CummuhandleSendData(task, (uint8*)"checkresp STARTRECORD", 22);

                { /* 生产中，在测试录音的时候，将耳机设置为单耳模式，后续的生产中才能正确进入底功耗 */
                    BtAddrPrmPtr prm = &gBtAddrParam;
                    if(FALSE == prm->single_era) {
                        prm->single_era = TRUE;
                        prm->peer_addr.addr.nap = 0xFFFF;
                        prm->peer_addr.addr.uap = 0xFF;
                        prm->peer_addr.addr.lap = 0xFFFFFF;
                        ParamSaveBtAddrPrm(prm);
                    }
                }

            }
            if(strstr((char *)payload, "check STARTRECORD1")){
#ifdef ENABLE_APOLLO
                apollo_sleep();
#endif
                progRun->apolloWakeup = 0;
                g_commuType        = 6;
                g_appConfigSocMic1 = NO_MIC;
                g_appConfigSocMic2 = 1;
                progRun->recStat  = 1;
#ifdef CONFIG_REC_ASSISTANT
                appKymeraRecordStart();
#endif
                MessageSendLater(task, COMMU_INTERVAL_RECORD, NULL, 10);

                CummuhandleSendData(task, (uint8*)"checkresp STARTRECORD", 22);
            }
            if(strstr((char *)payload, "check STOPRECORD")){
                g_commuType        = 0xFF;
                progRun->recStat  = 0;
#ifdef CONFIG_REC_ASSISTANT
                appKymeraRecordStop();
#endif
                CummuhandleSendData(task, (uint8*)"checkresp STOPRECORD", 22);
            }
            if(strstr((char *)payload, "check WAKEUP")){
                g_commuType = 1;
#ifdef ENABLE_APOLLO
                apollo_evoke();
#endif
                OperatorFrameworkEnable(MAIN_PROCESSOR_ON);
                appSubUISetMicbias(TRUE);
                CummuhandleSendData(task, (uint8*)"checkresp WAKEUP", 17);
            }
            if(strstr((char *)payload, "check RDSENSOR")){
#ifdef HAVE_EM20168
                if(EM20168_GetStatus() == 0)
                    value = EM20168_Get_psvalue();
#endif              
#ifdef HAVE_UCS146E0
                if(Ucs146e0_GetStatus() == 0)
                    value = Ucs146e0_get_ps_calvalue();
#endif
                outsize = sprintf(outbuf, "checkresp RDSENSOR=%d", value);
                CummuhandleSendData(task, (uint8*)outbuf, outsize);
            }

            if(strstr((char *)payload, "check WRSENSOR")){
                if((p = strchr((char *)payload, '=')) != NULL){
                    p += 1;
#ifdef HAVE_EM20168
                    if(EM20168_GetStatus() == 0)
                        EM20168_Set_psvalue(1, (unsigned short)atoi(p));
#endif
                    outsize = sprintf(outbuf, "checkresp WRSENSOR");
                    CummuhandleSendData(task, (uint8*)outbuf, outsize);
                }else{
                    CummuhandleSendData(task, (uint8*)"not find", 9);
                }
            }

            if(strstr((char *)payload, "check SENSOR")){
                g_commuType = 2;
                CummuhandleSendData(task, (uint8*)"checkresp SENSOR", 17);
            }

            if(strstr((char *)payload, "check PLC")){
                g_commuType = 3;
                CummuhandleSendData(task, (uint8*)"checkresp PLC", 14);
            }

            if(strstr((char *)payload, "check TAP")){
                g_commuType = 4;
                CummuhandleSendData(task, (uint8*)"checkresp TAP", 14);
            }

            if(strstr((char *)payload, "check SPEAKER")){
                CummuhandleSendData(task, (uint8*)"checkresp SPEAKER", 18);
                CummuPlayTone();
            }
            break;
        }

        case COMMU_INTERVAL_TIMER:
            outsize = CummuCheckAllItem(pComu, outbuf, sizeof(outbuf));
            if(outsize > 0){
                CummuhandleSendData(task, (uint8 *)outbuf, outsize);
                MessageSendLater(task, COMMU_INTERVAL_TIMER, NULL, 10);
            }
            if(pComu->type < TYPE_LAST)
                pComu->type  += 1;
            if(pComu->type == TYPE_LAST){
                CummuhandleSendData(task, (uint8*)"check ENDCHECK", 15);
                MessageCancelAll(task, COMMU_INTERVAL_TIMER);
                pComu->type == 0;
            }
            break;

        case COMMU_INTERVAL_RECORD:
            outsize = 0;
            CommuForwardSetDataClient();
            break;

        case GAIA_STAROT_COMMAND_IND:
            CommpcParse((GAIA_STAROT_AUDIO_IND_T *) message);
            break;
    }
}

void CommpcMessage(uint8* buff ,uint8 size)
{
    g_commuType = 0xFF;
    CummuhandleSendData(GetCommuTask(), (uint8 *)buff, size);
}

Task GetCommuTask(void)
{
    return &gCommInfo.task;
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

//////////////////////////////////////////////////////////////////////////////////////
///     PYDBG 调试使用
/////////////////////////////////////////////////////////////////////////////////////////
void do_rec(int start);
void do_play(int iNo);
void do_rec(int start)
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
    if(start)
        memcpy(message->payload,"check STARTRECORD0",19);
    else
        memcpy(message->payload,"check STOPRECORD0",18);
    MessageSend(GetCommuTask(), MESSAGE_FROM_HOST, message);
}

void do_play(int iNo)
{
    if(iNo >= 7)
        appUiPlayToneCore(commupc_tone, FALSE, TRUE, NULL, 0);
    else
        appUiPlayPrompt(PROMPT_POWER_ON+iNo);
}


#endif
