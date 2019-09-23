
#include <../av_headset.h>
#include <ps/ps_if.h>
#include <bdaddr.h>
#include <message.h>
#include <panic.h>
#include <ps.h>
#include <stdlib.h>
#include <string.h>
#include <vm.h>
#include <adv_manager.h>
#include "param.h"

#define PSKEY_CUSTOMER0_VALUE   (200)                         // ID, from trap_api/ps.h

#define PSKEY_PEER_ADDR         (PSKEY_CUSTOMER0_VALUE)       // 保存双方配对耳机蓝牙地址
#define PSKEY_BLE_PAIR          (PSKEY_CUSTOMER0_VALUE + 1)       // 保存ble配对信息adv广播，bind码

#define PSKEY_FIX_PARAM         (PSKEY_CUSTOMER0_VALUE)       // 固定参数
#define PSKEY_BTADDR_PARAM      (PSKEY_CUSTOMER0_VALUE+2)     // 蓝牙相关参数
#define PSKEY_USER_PARAM        (PSKEY_CUSTOMER0_VALUE+4)     // 用户设置参数

// 保存参数，如果length=0,表示擦除当前PSK中的参数
static int16 ParamSave(uint16 pskey, void *buffer, int length)
{
     uint16 ret, size = length/2;      // PsStore的第二个参数为words,因此长度应该除2

     ret = PsStore(pskey, buffer, size);
     if(ret != size)
         return -1;

     return size;
}

static int16 ParamLoad(uint16 pskey, void *buffer, int length)
{
     uint16 ret, size = length/2;      // PsStore的第二个参数为words,因此长度应该除2

     ret = PsRetrieve(pskey, buffer, size);
     if(ret != size)
         return -1;

     return size;
}

//////////////////////////////////////////////////////////////////////////////////
/////        出厂固定不变的参数
//////////////////////////////////////////////////////////////////////////////////
FixParam gFixParam;
void FixParamDefault(void)
{
    FixPrmPtr prm = &gFixParam;

    prm->aud_adj      = 0;
    prm->hw_ver       = 0xFFFF;
}

// taddr: save value;
//        NULL for delete old save
int16 ParamSaveFixPrm(FixPrmPtr pParam)
{
    FixPrmPtr prm = (pParam) ? pParam : &gFixParam;

    return ParamSave(PSKEY_FIX_PARAM, prm, sizeof(FixParam));
}

int16 ParamLoadFixPrm(FixPrmPtr pParam)
{
    FixPrmPtr prm = (pParam) ? pParam : &gFixParam;

    return ParamLoad(PSKEY_FIX_PARAM, prm, sizeof(FixParam));
}

/////////////////////////////////////////////////////////////////////////////////
/////// 蓝牙配对相关参数
/////////////////////////////////////////////////////////////////////////////////
BtAddrParam gBtAddrParam;
void BtAddrParamDefault(void)
{
    BtAddrPrmPtr prm = &gBtAddrParam;

    memset(prm, 0, sizeof(BtAddrParam));
    prm->single_era      = TRUE;
}

int16 ParamSaveBtAddrPrm(BtAddrPrmPtr pParam)
{
    BtAddrPrmPtr prm = (pParam) ? pParam : &gBtAddrParam;

    return ParamSave(PSKEY_BTADDR_PARAM, prm, sizeof(BtAddrParam));
}

int16 ParamLoadBtAddrPrm(BtAddrPrmPtr pParam)
{
    BtAddrPrmPtr prm = (pParam) ? pParam : &gBtAddrParam;

    return ParamLoad(PSKEY_BTADDR_PARAM, prm, sizeof(BtAddrParam));
}

/////////////////////////////////////////////////////////////////////////////////
//////// 一些用户可以修改的参数
/////////////////////////////////////////////////////////////////////////////////
UserParam gUserParam;
void UserParamDefault(void)
{
    UserPrmPtr prm = &gUserParam;

    memset(prm, 0, sizeof(UserParam));
}

int16 ParamSaveUserPrm(UserPrmPtr pParam)
{
    UserPrmPtr prm = (pParam) ? pParam : &gUserParam;

    return ParamSave(PSKEY_USER_PARAM, prm, sizeof(UserParam));
}

int16 ParamLoadUserPrm(UserPrmPtr pParam)
{
    UserPrmPtr prm = (pParam) ? pParam : &gUserParam;

    return ParamLoad(PSKEY_USER_PARAM, prm, sizeof(UserParam));

}


/////////////////////////////////////////////////////////////////////////////////
//////// 其它外部参数
/////////////////////////////////////////////////////////////////////////////////
#ifdef CONFIG_STAROT_SINGLE
// 检查是否为独立使用，没有耳机配对
// 单耳独立使用 0:daul 1:mon  ff:unknown
bool ParamUsingSingle(void)
{
    return (bool)gBtAddrParam.single_era;
}
#else
  #define ParamUsingSingle(...)       0
#endif

int16 ParamLoadAll(void)
{
    int16 ret = 0x07;

    if(ParamLoad(PSKEY_FIX_PARAM, &gFixParam, sizeof(FixParam)) < 0)
        FixParamDefault();
    else
        ret &= ~0x01;

    if(ParamLoad(PSKEY_BTADDR_PARAM, &gBtAddrParam, sizeof(BtAddrParam)) < 0)
        BtAddrParamDefault();
    else
        ret &= ~0x02;


    if(ParamLoad(PSKEY_USER_PARAM, &gUserParam, sizeof(UserParam)) < 0)
        UserParamDefault();
    else
        ret &= ~0x04;

    return ret;
}

int16 ParamGetPeerAddr( typed_bdaddr *taddr)
{
    memcpy(taddr, &gBtAddrParam.peer_addr, sizeof(typed_bdaddr));
    return sizeof(typed_bdaddr);
}

int16 ParamSaveBlePair(BlePairInfo *blePairInfo)
{
    memcpy(&gBtAddrParam.ble_pair, blePairInfo, sizeof(BlePairInfo));

    return ParamSaveBtAddrPrm(&gBtAddrParam);
}

int16 ParamLoadBlePair( BlePairInfo *blePairInfo)
{    
    memcpy(blePairInfo, &gBtAddrParam.ble_pair, sizeof(BlePairInfo));
    return sizeof(BlePairInfo);
}

// hwVer[short]+swVer[short]
int16 SystemGetVersion(uint8 *buffer)              // 获取软硬件版本信息
{
    uint16 *ptr = (uint16*)buffer;

    ptr[0] = gFixParam.hw_ver;
    ptr[1] = SYSTEM_SW_VERSION;

    return 4;       // BYTE
}



void ParamConfigInit(void)
{
    ParamLoadAll();

    /* Get local device address */
    ConnectionReadLocalAddr(appGetAppTask());
}

void ParamInitHandleClDmLocalBdAddrCfm(Message message)
{
    CL_DM_LOCAL_BD_ADDR_CFM_T *cfm = (CL_DM_LOCAL_BD_ADDR_CFM_T *)message;
    if (cfm->status != success)
        Panic();

    memcpy(&gProgRunInfo.addr, &cfm->bd_addr, sizeof(bdaddr));
    DEBUG_LOG("ParamInit addr %04x:%02x:%06x", cfm->bd_addr.nap, cfm->bd_addr.uap, cfm->bd_addr.lap);
}
