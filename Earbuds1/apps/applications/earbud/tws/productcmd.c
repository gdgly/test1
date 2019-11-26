
#include <Panic.h>
#include <stdlib.h>
#include <message.h>

#include "av_headset.h"

// 生产测试相关命令及涵数
void ProductEnterDutMode(void);
void ProductEnterReocrdMode(int16 isLeft);      // 1: left, 0:right
int appChangeCVCProcessMode(void);


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












