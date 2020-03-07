
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

//extern void appPhyStateSetState(phyStateTaskData* phy_state, phyState new_state);
static uint8 ProductPlayAudio(uint8 audNo)
 {
     if(audNo >= NUMBER_OF_PROMPTS)
         return 0xFF;

#if 10
     uiTaskData *theUi = appGetUi();
     const promptConfig *config = appConfigGetPromptConfig(audNo);
     FILE_INDEX *index = theUi->prompt_file_indexes + audNo;
     if (*index == FILE_NONE) {
        const char* name = config->filename;
        *index = FileFind(FILE_ROOT, name, strlen(name));
     }

     appKymeraPromptPlay(*index, config->format, config->rate,
            TRUE, NULL, 0);

#else
     appPhyStateSetState(appGetPhyState(), PHY_STATE_IN_EAR);
     appUiPlayPrompt(audNo);
#endif
     return audNo;
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
static void box_uc1460e_calc_cmd(uint8 *get_buf, uint8 *send_buf, uint8* static_buf);


void box_send_test_cmd(uint8 *get_buf, uint8 *send_buf)
{
    static uint8 Sn[16];
    uint8 buf_get;
    uint16 tmpval;
    (void)buf_get;
    FixPrmPtr prm = &gFixParam;

    send_buf[0] = get_buf[0];
    send_buf[1] = get_buf[1];  //需要返回值的话，给send_buf赋值
    send_buf[2] = 0x00;

    DEBUG_LOG("get_buf = %x:%x:%x",get_buf[0], get_buf[1], get_buf[2]);


    buf_get = get_buf[1];               //写耳机SN号
    if((buf_get>=0x20)&&(buf_get<= 0x2f))
    {
       Sn[buf_get - 0x20] = get_buf[2];

       if(buf_get == 0x2f)
        {
            SystemParamSn(0,Sn,TRUE);
        }
    }

    if((buf_get>=0x30)&&(buf_get<= 0x3f))   //读耳机SN号
    {
        SystemParamSn(0,Sn,FALSE);
        send_buf[2] = Sn[buf_get - 0x30];

    }
    if(get_buf[1] >= 0x48 && get_buf[1] <= 0x4F) {
        uint8 version[8];
        DevType type = appConfigIsLeft() ? DEV_LEFT : DEV_RIGHT;
        SystemGetVersion(type, version);
        send_buf[2] = version[get_buf[1] - 0x48];
        return;
    }

    if(get_buf[1] >= 0x6C && get_buf[1] <= 0x7F) {
        box_uc1460e_calc_cmd(get_buf, send_buf, Sn);
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
        case 0x0C:       // 播放指定的音频(与命令字0A差不多）
            send_buf[2] = ProductPlayAudio(get_buf[2]);
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

        case 0x60:         // 当前 耳机的 STATE 发给盒子，调试使用
            tmpval = (uint16)appGetState();
            send_buf[2] = (tmpval >> 8) & 0xFF;
            break;
        case 0x61:
            tmpval = (uint16)appGetState();
            send_buf[2] = (tmpval >> 0) & 0xFF;
            break;
        default:
            break;
    }
}

#include "ucs146e0.h"
static void box_uc1460e_calc_cmd(uint8 *get_buf, uint8 *send_buf, uint8* static_buf)
{
#ifdef HAVE_UCS146E0
    uint16 value;
    FixPrmPtr prm = &gFixParam;

    switch(get_buf[1]) {
    case 0x6C:
        send_buf[2] = Ucs146e0_statcheck();
        break;

        // 发送初始化命令，每次三个字节
    case 0x6D:
        send_buf[2] = 1;
        static_buf[0] = get_buf[1];
        break;
    case 0x6E:
        send_buf[2] = 2;
        static_buf[1] = get_buf[1];
        break;
    case 0x6F:         // 开始启动并初始化
        send_buf[2] = get_buf[2];
        if(1 == send_buf[2]) {
            Ucs146e0Power(TRUE);
            Ucs146e0_get_crosstalk_init();
        }
        else if(2 == send_buf[2] || 3 == send_buf[2])
            Ucs146e0_get_ps_cal_init((static_buf[0<<8] | static_buf[1]));
        else
            send_buf[2] = 0;
        break;

        // 读取talk值
    case 0x70:
        value = Ucs146e0_get_crosstalk_calvalue();
        static_buf[2] = (value >> 8 ) & 0xFF;
        static_buf[3] = (value >> 0 ) & 0xFF;
        send_buf[2] = static_buf[2];
        break;
    case 0x71:
        send_buf[2] = static_buf[3];
        break;

        // 读取 PS value
    case 0x72:
        value = Ucs146e0_get_ps_calvalue();
        static_buf[4] = (value >> 8 ) & 0xFF;
        static_buf[5] = (value >> 0 ) & 0xFF;
        send_buf[2] = static_buf[4];
        break;
    case 0x73:
        send_buf[2] = static_buf[5];
        break;

        // 读取参数
    case 0x74:
        memset(&static_buf[8], 0, 6);     // 需要先取出参数到static_buf中
        if(prm->em20168_cal_already) {
            static_buf[8] = (prm->em20168_high_value >> 8) & 0xFF;
            static_buf[9] = (prm->em20168_high_value >> 0) & 0xFF;
            static_buf[10] = (prm->em20168_low_value >> 8) & 0xFF;
            static_buf[11] = (prm->em20168_low_value >> 0) & 0xFF;
            static_buf[12] = (prm->ucs146e0_calib >> 8) & 0xFF;
            static_buf[13] = (prm->ucs146e0_calib >> 0) & 0xFF;
        }        // 不要加break
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x78:
    case 0x79:
        value = get_buf[1] - 0x74;
        send_buf[2] = static_buf[value+8];
        break;

        // 写参数
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
        value = get_buf[1] - 0x7A;
        static_buf[value+8] = get_buf[2];        // 因为是分多次发送过来，需要先保存到static_buf中
        if(get_buf[1] == 0x7F) {
            prm->em20168_high_value = (static_buf[8] << 8) & static_buf[9];
            prm->em20168_low_value  = (static_buf[10] << 8) & static_buf[11];
            prm->ucs146e0_calib     = (static_buf[12] << 8) & static_buf[13];
            prm->em20168_cal_already = 1;
            ParamSaveFixPrm(prm);
        }
        break;
    }
#else
    (void)get_buf, (void)send_buf, (void)static_buf;
#endif
}
