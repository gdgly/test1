#include <stdio.h>
#include <sink.h>
#include <source.h>
#include <stream.h>
#include <operator.h>
#include <panic.h>
#include <pio.h>
#include <ps.h>
#include <string.h>
#include <os.h>
#include <message.h>
#include <vmal.h>
#include <pmalloc.h>
#include <panic.h>
#include <app/message/system_message.h>
#include <../av_headset.h>

#include "public.h"

#ifdef CONFIG_PSRAM_TEST
static int psram_power(bool isOn)
{
    int ret = 0;
    // Set IO
    ret |= PioSetMapPins32Bank(PIO2BANK(PSRAM_POWER_PIO), PIO2MASK(PSRAM_POWER_PIO), PIO2MASK(PSRAM_POWER_PIO));
    ret |= PioSetDir32Bank(PIO2BANK(PSRAM_POWER_PIO), PIO2MASK(PSRAM_POWER_PIO), PIO2MASK(PSRAM_POWER_PIO));

    ret |= PioSet32Bank(PIO2BANK(PSRAM_POWER_PIO), PIO2MASK(PSRAM_POWER_PIO),
                 isOn ? PIO2MASK(PSRAM_POWER_PIO) : 0);

    return ret;
}
#endif


#ifdef CONFIG_SHELL_UART
#define MAX_CMD_LEN         (64)
#define MSG_GET_COMMAND     (0xFF00)

typedef struct tagSHELLCMDINFO {
    TaskData       task;

    unsigned short cmdlen, datlen;
    uint8          data[MAX_CMD_LEN];
}ShellInfoTask;

static ShellInfoTask *pShellTask = NULL;

// 注意每次UART接收到的数据不一定是一个完整的命令行
// 需要自己累加到data中去
static signed UartRxCallback(const uint8 *ptr, int len)
{
    int i = 0, count;

    if(!pShellTask)
        return -len;

    count = pShellTask->datlen;
    for(i = 0; i < len; i++) {
        pShellTask->data[count] = ptr[i];
        if(((ptr[i] == '\n') || (ptr[i] == '\0')) && (pShellTask->cmdlen == 0)) {
            if(count < 2) {
                count = 0;
                continue;
            }
            else {
                pShellTask->cmdlen = count;
                pShellTask->data[count] ='\0';
            }
        }

        count += 1;
        if(count >= MAX_CMD_LEN) {
            pShellTask->cmdlen = 0;
            count = 0;
        }
    }

    pShellTask->datlen = count;
    if(pShellTask->cmdlen > 0) {
        MessageSend(&pShellTask->task, MSG_GET_COMMAND, NULL);
    }

    return len;
}

extern void connectionAuthDeleteDeviceFromTdl(const TYPED_BD_ADDR_T *addrt);
void DoBdAddr(int type);

// type:0,showaddr, 1:clearaddr
void DoBdAddr(int type)
{
    char outbuf[64] = {'\0'};
    uint16 i, count = ConnectionTrustedDeviceListSize();

    if(0 == type) {
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice\n");
        }
        else {
            typed_bdaddr taddr;
            for(i = 0; i < count; i++ ) {
                if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, NULL, &taddr) == TRUE) {
                    sprintf(outbuf, "addr=%d/%d t=%x lun=%06x:%02x:%04x\n", i, count, taddr.type,
                            taddr.addr.lap, taddr.addr.uap, taddr.addr.uap);
                    sprintf("%s", outbuf);
                    outbuf[0] = '\0';
                }
                else {
                    sprintf(outbuf, "addr=%d/%d Error\n", i, count);
                    break;
                }
            }
        }
    }
    else if(1 == type) {
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice Clear\n");
        }
        else {
            typed_bdaddr taddr;
            for(i = 0; i < count; i++ ) {
                if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, NULL, &taddr) == TRUE) {
                    connectionAuthDeleteDeviceFromTdl((TYPED_BD_ADDR_T *)&taddr);
                }
                sprintf(outbuf, "Clear TrustedDevice %d/%d\n", i, count);
            }
        }
    }
    if(outbuf[0] != '\0') {
        sprintf("%s", outbuf);
    }
}


static void ShellDoCommand(char *buffer, int len)
{
    int ret, what = 0;     // 0:close/off  1:open/on
    char outbuf[64] = {'\0'};

    UNUSED(len);

    if(strstr(buffer, " on") || strstr(buffer, " open") || strstr(buffer, " yes"))
        what = 1;

    if(strstr(buffer, "lis25")) {          /* LIS25 开关测试命令 */
        ret  = lis25Power(what);
        ret |= lis25TestPower(what);
        sprintf(outbuf, "Set Lis25[%d] ret=%d\n", what, ret);
    }
#ifdef CONFIG_PSRAM_TEST
    else if(strstr(buffer, "psram")) {
        ret = psram_power(what);
        sprintf(outbuf, "Set Psram[%d] ret=%d\n", what, ret);
    }
#endif
    else if(strstr(buffer, "bma400")) {
        ret = BMA400Power(what);
        sprintf(outbuf, "Set bma400[%d] ret=%d\n", what, ret);
    }
    else if(strstr(buffer, "em20168")) {
        ret = EM20168Power(what);
        sprintf(outbuf, "Set em20168[%d] ret=%d\n", what, ret);
    }
    else if(strstr(buffer, "showaddr")) {
        uint16 i, count = ConnectionTrustedDeviceListSize();
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice\n");
        }
        else {
            typed_bdaddr taddr;
            for(i = 0; i < count; i++ ) {
                if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, NULL, &taddr) == TRUE) {
                    sprintf(outbuf, "addr=%d/%d t=%x lun=%06x:%02x:%04x\n", i, count, taddr.type,
                            taddr.addr.lap, taddr.addr.uap, taddr.addr.uap);
                    UartTxData((const uint8*)outbuf, strlen(outbuf));
                    outbuf[0] = '\0';
                }
                else {
                    sprintf(outbuf, "addr=%d/%d Error\n", i, count);
                    break;
                }
            }
        }
    }
    else if(strstr(buffer, "clearaddr")) {
        uint16 i, count = ConnectionTrustedDeviceListSize();
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice Clear\n");
        }
        else {
            typed_bdaddr taddr;
            for(i = 0; i < count; i++ ) {
                if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, NULL, &taddr) == TRUE) {
                    connectionAuthDeleteDeviceFromTdl((TYPED_BD_ADDR_T *)&taddr);
                }
                sprintf(outbuf, "Clear TrustedDevice %d/%d\n", i, count);
            }
        }
    }

    if(outbuf[0] != '\0')
        UartTxData((const uint8*)outbuf, strlen(outbuf));
}

// 将前面已经使用的数据丢弃，剩下的数据移动一开始处
static void ShellMoveData(ShellInfoTask *theTask)
{
    if(theTask->datlen > theTask->cmdlen) {
        short less = theTask->datlen - theTask->cmdlen;
        short  from = theTask->cmdlen;
        theTask->datlen = theTask->cmdlen = 0;
        UartRxCallback(&theTask->data[from], less);
        MessageSend(&pShellTask->task, MSG_GET_COMMAND, NULL);
    }
    else {
        theTask->datlen = theTask->cmdlen = 0;
    }
}


static void ShellMessageHandler (Task pTask, MessageId pId, Message pMessage)
{
    UNUSED(pMessage);
    ShellInfoTask *theTask = (ShellInfoTask*)pTask;
    switch (pId) {
    case MSG_GET_COMMAND:
        if(theTask->cmdlen > 0) {
            ShellDoCommand((char*)theTask->data, theTask->cmdlen);
            ShellMoveData(theTask);
        }
        break;

    }
}

void ShellCmdInit(void)
{
    pShellTask = PanicUnlessNew(ShellInfoTask);

    memset(pShellTask, 0, sizeof(ShellInfoTask));
    pShellTask->task.handler = ShellMessageHandler;

    UartSetRecvHandle(&UartRxCallback);
}
#endif
