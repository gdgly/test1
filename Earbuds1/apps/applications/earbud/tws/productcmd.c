
#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "av_headset.h"
#include "public.h"
// 生产测试相关命令及涵数
void ProductEnterDutMode(void);
void ProductEnterReocrdMode(int16 isLeft);      // 1: left, 0:right
int appChangeCVCProcessMode(void);
void appEnterSingleForTest(void);
void appSetSingleModeTest(void);

 #define RINGTONE_STOP  RINGTONE_NOTE(REST, HEMIDEMISEMIQUAVER), RINGTONE_END
 static const ringtone_note product_tone[] =
 {
     RINGTONE_TIMBRE(sine), RINGTONE_DECAY(50),
     RINGTONE_NOTE(A9, SEMIBREVE),
     RINGTONE_NOTE(A9, SEMIBREVE),
     RINGTONE_NOTE(A9, SEMIBREVE),
 //    RINGTONE_NOTE(D9, SEMIBREVE),
 //    RINGTONE_NOTE(E9, SEMIBREVE),
 //    RINGTONE_NOTE(F9, SEMIBREVE),
 //    RINGTONE_NOTE(G9, SEMIBREVE),
     RINGTONE_STOP
 };

 static void ProductPlayTone(void)
 {
 //   appUiPlayToneCore(product_tone, FALSE, TRUE, NULL, 0);
     appKymeraTonePlay(product_tone, TRUE, NULL, 0);
 }

//==================================================================================
// 蓝牙传导测试时，可以通过盒子发送集合执行这个函数。
// 然后测试设备就可以与析卡进行通信，并执行对应的测试
// DUT模式也可以通过bluesuit\BlueTest3 界面控制进入
//==================================================================================
void ProductEnterDutMode(void)
{
    ConnectionEnterDutMode();
}

//==================================================================================
// MIC测试时，通过经典蓝牙。需要通过左右声道分别录音。
// 我们可以通过CVC来控制录音通道
// 系统在启动SOC时会来调用 appChangeCVCProcessMode
//==================================================================================
#define CVC_PROCESSING_MODE_STATIC         		0
#define CVC_PROCESSING_MODE_STANDBY        		1
#define CVC_PROCESSING_MODE_FULL           		2
#define CVC_PROCESSING_MODE_LOWVOLUME      		3
#define CVC_PROCESSING_MODE_PASS_THRU_LEFT 		4
#define CVC_PROCESSING_MODE_PASS_THRU_RIGHT	        5
#define CVC_PROCESSING_MODE_PASS_THRU_MIC3 		6
#define CVC_PROCESSING_MODE_PASS_THRU_MIC4 		7
#define CVC_PROCESSING_MODE_MAX_MODES      		8
static uint8 g_cvcMode = CVC_PROCESSING_MODE_FULL;             // 默认一定是双MIC降噪模式

void ProductEnterReocrdMode(int16 isLeft)
{
    g_cvcMode = (isLeft) ? CVC_PROCESSING_MODE_PASS_THRU_LEFT : CVC_PROCESSING_MODE_PASS_THRU_RIGHT;
}

#include <av_headset_kymera_private.h>
int appChangeCVCProcessMode(void)
{
    if(CVC_PROCESSING_MODE_FULL == g_cvcMode)
        return 0;

    kymera_chain_handle_t scochain = appKymeraGetScoChain();
    if (scochain)   {
         Operator op = ChainGetOperatorByRole(scochain, OPR_CVC_SEND);
          if(op) {
             OperatorsStandardSetControl(op, OPMSG_CONTROL_MODE_ID, g_cvcMode);
             return 0;
          }
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////
//  部分命令需要使用TASK来处理
///////////////////////////////////////////////////////////////////////////
typedef struct tagPRODUCTCMDINFO {
    TaskData       task;
    uint8          tmp_sn[20];
}ProductCmdInfo, *ProdCmdIPtr;
ProdCmdIPtr _ProdCmdPtr = NULL;

#define  PRODCMD_HANDSET_PAIR_STEP1   0x2500
#define  PRODCMD_HANDSET_PAIR_STEP2   0x2501
#define  PRODCMD_HANDSET_PAIR_STEP3   0x2502

static void ProductTaskHandleMessage(Task task, MessageId id, Message message)
{
    (void)task;(void)message;
    switch(id) {
    case PRODCMD_HANDSET_PAIR_STEP1:
        appSetState(APP_STATE_IN_CASE_IDLE);
        MessageSendLater(&_ProdCmdPtr->task, PRODCMD_HANDSET_PAIR_STEP2, 0, 150);
        break;
    case PRODCMD_HANDSET_PAIR_STEP2:
        appSmPairHandset();
        MessageSendLater(&_ProdCmdPtr->task, PRODCMD_HANDSET_PAIR_STEP3, 0, 150);
        break;
    case PRODCMD_HANDSET_PAIR_STEP3:
        MessageSend(appGetUiTask(), APP_ATTACH_PLC_OUT, NULL);
        break;
    }
}

// 启动广播，供手机连接。耳机可能处在双耳配对过程中，需要多条指令来切换到对应的状态
void appEnterSingleForTest(void)
{
#if 1
    if(NULL == _ProdCmdPtr) {
        _ProdCmdPtr = PanicUnlessNew(ProductCmdInfo);
        _ProdCmdPtr->task.handler = ProductTaskHandleMessage;
    }

    appSetState(APP_SUBSTATE_TERMINATING);
    MessageSendLater(&_ProdCmdPtr->task, PRODCMD_HANDSET_PAIR_STEP1, 0, 100);

#else
    BtAddrPrmPtr prm = &gBtAddrParam;

    prm->single_era = TRUE;
    prm->peer_addr.addr.nap = 0xFFFF;
    prm->peer_addr.addr.uap = 0xFF;
    prm->peer_addr.addr.lap = 0xFFFFFF;
    // don't save

    appConManagerAllowHandsetConnect(TRUE);
    appSmSetUserPairing();
    appSetState(APP_STATE_HANDSET_PAIRING);
#endif
}

void appSetSingleModeTest(void)
{
    BtAddrPrmPtr prm = &gBtAddrParam;
    prm->single_era = TRUE;
    prm->peer_addr.addr.nap = 0xFFFF;
    prm->peer_addr.addr.uap = 0xFF;
    prm->peer_addr.addr.lap = 0xFFFFFF;
}

#ifdef ENABLE_APOLLO
extern int apolloGetStatus(void);
extern int apollo_evoke(void);
#endif
extern void appSubUISetMicbias(int set);


void box_send_test_cmd(uint8 *get_buf, uint8 *send_buf)
{
    uint8 buf_get;
    uint16 tmpval;
    (void)buf_get;
    FixPrmPtr prm = &gFixParam;

    send_buf[0] = get_buf[0];
    send_buf[1] = get_buf[1];  //需要返回值的话，给send_buf赋值
    send_buf[2] = 0x00;

    DEBUG_LOG("get_buf = %x:%x:%x",get_buf[0], get_buf[1], get_buf[2]);

    buf_get = get_buf[1];
    if((buf_get>=0x20)&&(buf_get<= 0x2f))
    {
       prm->sn[buf_get - 0x20] = get_buf[2];

       if(buf_get == 0x2f)
        {
            ParamSaveSN(prm->sn);
        }
    }

    if((buf_get>=0x30)&&(buf_get<= 0x3f))
    {
        ParamLoadSN(prm->sn);
        send_buf[2] = prm->sn[buf_get - 0x30];

    }

    if(get_buf[1] >= 0x48 && get_buf[1] <= 0x4F) {
        uint8 version[8];
        DevType type = appConfigIsLeft() ? DEV_LEFT : DEV_RIGHT;
        SystemGetVersion(type, version);
        send_buf[2] = version[get_buf[1] - 0x48];
        return;
    }

    switch(get_buf[1])
    {
        case 0x00:   //复位右
            send_buf[2] = 0x01;
        case 0x01:   //复位左
            appSetState(APP_STATE_FACTORY_RESET);
            appSmFactoryReset();
            break;
        case 0x07:   // 启动或获取唤醒状态
            if(0x00 == get_buf[2]) {
#ifdef ENABLE_APOLLO
                apollo_evoke();
#endif
                OperatorFrameworkEnable(MAIN_PROCESSOR_ON);
                appSubUISetMicbias(TRUE);
                send_buf[2] = 0x00;
                appSubGetProgRun()->iWakeupTimes = 0;
            }
            else if(0x01 == get_buf[2]){
                send_buf[2] = appSubGetProgRun()->iWakeupTimes;
            }
            break;
        case 0x08:   //主MIC
            ProductEnterReocrdMode(1);
            break;
        case 0x09:   //副MIC
            ProductEnterReocrdMode(0);
            break;
        case 0x0a:   //SPEAK
            ProductPlayTone();
            break;
        case 0x0e:   //蓝牙
            ProductEnterDutMode();
            break;
        case 0x0B:   // check device
#ifdef HAVE_EM20168
        send_buf[2] |= (EM20168_GetStatus() == 0) ? 0x01 : 0;
#endif
#ifdef HAVE_LIS2DW12
        send_buf[2] |= (lis2dw12_GetStatus() == 0) ? 0x02 : 0;
#endif
#ifdef HAVE_MAX20340
        send_buf[2] |= (max20340_GetStatus() == 0) ? 0x04 : 0;
#endif
#ifdef ENABLE_APOLLO
        send_buf[2] |= (apolloGetStatus() == 0) ? 0x08 : 0;
#endif
#ifdef CONFIG_BOARD_V2_LIS25
        send_buf[2] |= (lis25GetStatus() == 0) ? 0x10 : 0;
#endif
            break;

        case 0x0f:
            appEnterSingleForTest();
            break;
        case 0x10:   //读接近光校准高_H
            send_buf[1] = 0x10;
            send_buf[2] = prm->em20168_high_value>>8;
            break;
        case 0x11:   //读接近光校准高_L
            send_buf[1] = 0x11;
            send_buf[2] = prm->em20168_high_value&0xff;
            break;
        case 0x40:   //读接近光校准低_H
            send_buf[1] = 0x40;
            send_buf[2] = prm->em20168_low_value>>8;
            break;
        case 0x41:   //读接近光校准低_L
            send_buf[1] = 0x41;
            send_buf[2] = prm->em20168_low_value&0xff;
            break;
        case 0x12:   //写接近光校准高_H
            prm->em20168_cal_already = 0;         // 准备写设置为非校准
            prm->em20168_high_value = get_buf[2]<<8;
            send_buf[1] = 0x12;  //需要返回值的话，给send_buf赋值
            send_buf[2] = 0x00;
            break;
        case 0x13:   //写接近光校准高_L
            prm->em20168_high_value |= get_buf[2];
            send_buf[1] = 0x13;  //需要返回值的话，给send_buf赋值
            send_buf[2] = 0x00;
            break;
        case 0x42:   //写接近光校准低_H
            prm->em20168_low_value = get_buf[2]<<8;
            send_buf[1] = 0x42;  //需要返回值的话，给send_buf赋值
            send_buf[2] = 0x00;
            break;
        case 0x43:   //写接近光校准低_L，全部写完
            prm->em20168_low_value |= get_buf[2];
            prm->em20168_cal_already = 1;
            ParamSaveFixPrm(NULL);
            send_buf[1] = 0x43;  //需要返回值的话，给send_buf赋值
            send_buf[2] = 0x00;
            break;

        case 0x44:
            tmpval = EM20168_Get_psvalue();
            send_buf[2] = (tmpval >> 8) & 0xFF;
            break;
        case 0x45:
            tmpval = EM20168_Get_psvalue();
            send_buf[2] = (tmpval >> 0) & 0xFF;
            break;

        case 0x14:   //接近光验证打开
            appSubGetProgRun()->iPowerSaveMode = POWER_MODE_IN_EAR;
            appUiPowerSaveSync();
            break;
        case 0x15:   //接近光验证状态
            send_buf[1] = 0x15;//需要返回值的话，给send_buf赋值
            send_buf[2] = EM20168_statcheck();
            break;
        default:
            break;
    }
}
