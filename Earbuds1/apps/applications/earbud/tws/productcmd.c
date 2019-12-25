
#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "av_headset.h"

// 生产测试相关命令及涵数
void ProductEnterDutMode(void);
void ProductEnterReocrdMode(int16 isLeft);      // 1: left, 0:right
int appChangeCVCProcessMode(void);

 uint8 SNsize20=0;
 uint8 SNsize21=0;
 uint8 SNsize30=0;
 uint8 SNsize31=0;

 #define RINGTONE_STOP  RINGTONE_NOTE(REST, HEMIDEMISEMIQUAVER), RINGTONE_END
 static const ringtone_note product_tone[] =
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

 static void ProductPlayTone(void)
 {
     appUiPlayToneCore(product_tone, FALSE, TRUE, NULL, 0);
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

static void appEnterSingleforTest(void)
{
#if 0
    typed_bdaddr taddr;

    taddr.addr.nap = 0xFFFF;
    taddr.addr.uap = 0xFF;
    taddr.addr.lap = 0xFFFFFF;
    ParamSavePeerAddr(&taddr);
    PanicNotZero(1);           // 系统重新启动一下
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


void box_send_test_cmd(uint8 *get_buf, uint8 *send_buf)
{
//    uint8 i;
    FixPrmPtr prm = &gFixParam;

    DEBUG_LOG("get_buf = %x:%x:%x\n",get_buf[0], get_buf[1], get_buf[2]);
    switch(get_buf[1])
    {
        case 0x00:   //复位右
            appSmFactoryReset();
            send_buf[1] = 0x00;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0x01;
            break;
        case 0x01:   //复位左
            appSmFactoryReset();
            send_buf[1] = 0x01;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            break;
        case 0x08:   //主MIC
            ProductEnterReocrdMode(1);
            send_buf[1] = 0x08;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            appEnterSingleforTest();
            break;
        case 0x09:   //副MIC
            ProductEnterReocrdMode(0);
            send_buf[1] = 0x09;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            appEnterSingleforTest();
            break;
        case 0x0a:   //SPEAK
            ProductPlayTone();
            send_buf[1] = 0x0a;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            appEnterSingleforTest();
            break;
        case 0x0e:   //蓝牙
            ProductEnterDutMode();
            send_buf[1] = 0x0e;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            appEnterSingleforTest();
            break;
        case 0x10:   //接近光校准读高
            prm->em20168_high_value = EM20168_Get_psvalue();
            DEBUG_LOG("em20168_high_value = %x\n",prm->em20168_high_value);
            send_buf[1] = 0x10;//需要返回值的话，给send_buf赋值
            send_buf[2] = prm->em20168_high_value>>8;
            break;
        case 0x11:   //接近光校准读低
            prm->em20168_low_value = EM20168_Get_psvalue();
            DEBUG_LOG("em20168_low_value = %x\n",prm->em20168_low_value);
            send_buf[1] = 0x11;//需要返回值的话，给send_buf赋值
            send_buf[2] = prm->em20168_low_value&0xff;
            break;
        case 0x12:   //接近光校准写高
            prm->em20168_high_value = get_buf[2]<<8;
            EM20168_Set_psvalue(1,prm->em20168_high_value);
            send_buf[1] = 0x12;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
              break;
        case 0x13:   //接近光校准写低
            prm->em20168_low_value = get_buf[2];
            EM20168_Set_psvalue(0,prm->em20168_low_value);
            send_buf[1] = 0x13;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
              break;
        case 0x14:   //接近光验证打开
            EM20168Power(1);
            send_buf[1] = 0x14;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
              break;
        case 0x15:   //接近光验证状态
//          EM20168_itr_read_reg(task, id, msg);
//            em20168_timer_restart(100);
            send_buf[1] = 0x15;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            break;
        case 0x20:   //写SN号
            prm->sn[SNsize20] = get_buf[2];
            SNsize20++;
            if(SNsize20>15)
            {
                ParamSaveSN(prm->sn);
                SNsize20 = 0;
            }
            send_buf[1] = 0x20;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
            break;
        case 0x21:   //写SN号
            prm->sn[SNsize21] = get_buf[2];
            SNsize21++;
            if(SNsize21>15)
            {
                ParamSaveSN(prm->sn);
                SNsize21 = 0;
            }
            send_buf[1] = 0x21;//需要返回值的话，给send_buf赋值
            send_buf[2] = 0;
              break;
        case 0x30:   //读SN号
            if(!SNsize30)   ParamLoadSN(prm->sn);
            send_buf[1] = 0x30;//需要返回值的话，给send_buf赋值
            send_buf[2] = prm->sn[SNsize30];
            SNsize30++;
            if(SNsize30>15) SNsize30=0;
              break;
        case 0x31:   //读SN号
            if(!SNsize31)   ParamLoadSN(prm->sn);
            send_buf[1] = 0x31;//需要返回值的话，给send_buf赋值
            send_buf[2] = prm->sn[SNsize31];
//          DEBUG_LOG("sen_buf = %x\n",send_buf[2]);
            SNsize31++;
            if(SNsize31>15) SNsize31=0;
            break;
    }
    send_buf[0] = get_buf[0];
}









