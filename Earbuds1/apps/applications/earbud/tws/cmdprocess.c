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

#include <bluestack/dm_prim.h>
static void ShellDoCommand(char *buffer, int len);
extern void connectionAuthDeleteDeviceFromTdl(const TYPED_BD_ADDR_T *addrt);
extern void connectionAuthUpdateTdl(const TYPED_BD_ADDR_T  *addrt, const DM_SM_KEYS_T *keys );
//extern ConnectionSmPutAttribute(0, bd_addr, sizeof(*attributes), (uint8 *)attributes);


// type: 0:显示当前保存的BtAddr
//       1:清除当前保存的ADDR
//       2:增加一个ADDR
static void DoBtAddrFunc(int type, char *outbuf, char *param)
{
    DM_SM_KEYS_T keys;
    appDeviceAttributes attributes;
    typed_bdaddr taddr = {0, {0x00ff01,0x5b,0x0002}};
    uint16 i, count = ConnectionTrustedDeviceListSize();

    (void)param;(void)attributes;
    switch(type) {
    case 0:          // SHOW
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice\n");
            break;
        }

        for(i = 0; i < count; i++ ) {
            if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, (uint8 *)&attributes, &taddr) == TRUE) {
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
        break;

   case 1:          // CLEAR
        if(0 == count) {
            sprintf(outbuf, "No TrustedDevice Clear\n");
            break;
        }

        for(i = 0; i < count; i++ ) {
            if(ConnectionSmGetIndexedAttributeNowReq(0, i, 0, NULL, &taddr) == TRUE) {
                connectionAuthDeleteDeviceFromTdl((TYPED_BD_ADDR_T *)&taddr);
                memset(outbuf, 0, 16);
                ConnectionPutDatabaseHash(16, (uint8*)outbuf);
            }
            sprintf(outbuf, "Clear TrustedDevice %d/%d\n", i, count);
            UartTxData((const uint8*)outbuf, strlen(outbuf));
            outbuf[0] = '\0';
        }
        break;

    case 2:
        appDeviceInitAttributes(&attributes);
        attributes.type             = DEVICE_TYPE_EARBUD;
        attributes.tws_version      = DEVICE_TWS_STANDARD;
#if 0
        attributes.a2dp_num_seids   = 0x00;
        attributes.a2dp_volume      = 0x62;
        attributes.hfp_profile      = 0x04;
        attributes.dev_info_version = 0x01;
        attributes.link_mode        = DEVICE_LINK_MODE_NO_SECURE_CONNECTION;
        attributes.supported_profiles = 0x07;
        attributes.connected_profiles = 0x03;
#endif
        taddr.addr.lap = 0x00ff0a;
        memset(&keys, 0, sizeof(keys));
        connectionAuthUpdateTdl((const TYPED_BD_ADDR_T *)&taddr, &keys);
        ConnectionSmPutAttribute(0, &taddr.addr, sizeof(attributes), (uint8 *)&attributes);
        UartTxData((const uint8*)"Add AddrL\n", 9);
        DoBtAddrFunc(0, outbuf, NULL);
        break;
    case 3:
        taddr.addr.lap = 0x00ff09;
        memset(&keys, 0, sizeof(keys));
        connectionAuthUpdateTdl((const TYPED_BD_ADDR_T *)&taddr, &keys);

        UartTxData((const uint8*)"Add AddrR\n", 9);
        DoBtAddrFunc(0, outbuf, NULL);
        break;
    }
}


static void ShellDoCommand(char *buffer, int len)
{
    int ret, what = 0;     // 0:close/off  1:open/on
    char outbuf[64] = {'\0'}, *param = NULL;

    UNUSED(len);

    if((param = strstr(buffer, " ")) != NULL) {
        while(param[0] == ' ')
            param += 1;
    }

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
    else if(strstr(buffer, "addrshow"))
        DoBtAddrFunc(0, outbuf, param);
    else if(strstr(buffer, "addrclear"))
        DoBtAddrFunc(1, outbuf, param);
    else if(strstr(buffer, "addraddl"))
        DoBtAddrFunc(2, outbuf, param);
    else if(strstr(buffer, "addraddr"))
        DoBtAddrFunc(3, outbuf, param);
    else
        sprintf(outbuf, "Unknown %s\n", buffer);


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
