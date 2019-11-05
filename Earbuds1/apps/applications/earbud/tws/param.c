
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



/* SW版本共4字节： 00.01.01.xx   xx为测试使用*/
const uint8 SYSTEM_SW_VERSION[DEV_SWVER_LEN] = {0x00,0x01,0x02,0x24};   // 每次发布版本修改这儿的值

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
    prm->hw_ver[0]    = 0xE0;
    prm->hw_ver[1]    = 0x01;
    prm->hw_ver[2]    = 0x00;
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

    // 其它版本信息设置为无效
    memset(prm->peerVer, 0xFF, DEV_HWSWVER_LEN);
    memset(prm->caseVer, 0xFF, DEV_HWSWVER_LEN);
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
static UserPrmPtr GetUserParam(void)
{
    return &gUserParam;
}

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
bool ParamUsingSingle(void) { return TRUE; }
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

// 获取软硬件版本信息
// hwVer[3]+rev[1]+swVer[4]
// type: 0：盒子， 1：对方耳机(右），2：设备自身(左）
int16 SystemGetVersion(DevType type, uint8 *buffer)
{
    uint8 *ptr = (uint8*)buffer;

    if(DEV_CASE == type)
        memcpy(ptr, gBtAddrParam.caseVer, DEV_HWSWVER_LEN);
    else if((DEV_LEFT == type && appConfigIsLeft())||(DEV_RIGHT == type && appConfigIsRight())) {
        memcpy(ptr, gFixParam.hw_ver, DEV_HWVER_LEN);
        ptr[DEV_HWVER_LEN] = ' ';
        memcpy(&ptr[DEV_HWVER_LEN+1], SYSTEM_SW_VERSION, DEV_SWVER_LEN);
    }
    else {
        memcpy(ptr, gBtAddrParam.peerVer, DEV_HWSWVER_LEN);
    }

    return DEV_HWSWVER_LEN;       // BYTE
}

int16 SystemSetVersion(DevType type, uint8 *buffer)
{
    uint8 *ptr = (uint8*)buffer;
    if(DEV_CASE == type)
        memcpy(gBtAddrParam.caseVer, ptr, DEV_HWSWVER_LEN);
    else if((DEV_LEFT == type && appConfigIsLeft())||(DEV_RIGHT == type && appConfigIsRight())) {
       // 自己的版本号不能更改
       // memcpy(gFixParam.hw_ver, ptr, DEV_HWVER_LEN);
    }
    else {
        memcpy(gBtAddrParam.peerVer, ptr, DEV_HWSWVER_LEN);
    }
    return DEV_HWSWVER_LEN;

}

int16 UserGetKeyFunc(uint8 *lKeyFunc, uint8 *rKeyFunc)     // 获取功能键
{
    UserPrmPtr prm = GetUserParam();

    if(lKeyFunc)
        *lKeyFunc = prm->lKeyFunc;

    if(rKeyFunc)
        *rKeyFunc = prm->rKeyFunc;

    return 0;
}

int16 UserSetKeyFunc(uint8 lKeyFunc, uint8 rKeyFunc)     // 设置功能键
{
    UserPrmPtr prm = GetUserParam();
    if(0XFF != lKeyFunc) {
        prm->lKeyFunc = lKeyFunc;
    }
    if(0XFF != rKeyFunc) {
        prm->rKeyFunc = rKeyFunc;
    }
    ParamSaveUserPrm(prm);
    return 0;
}


extern bool appAdvParamInit(void);

void ParamConfigInit(void)
{
    ParamLoadAll();

    /* Get local device address */
    ConnectionReadLocalAddr(appGetAppTask());

    /* 广播数据 */
    appAdvParamInit();
}

void ParamInitHandleClDmLocalBdAddrCfm(Message message)
{
    ProgRIPtr  progRun = appSubGetProgRun();
    CL_DM_LOCAL_BD_ADDR_CFM_T *cfm = (CL_DM_LOCAL_BD_ADDR_CFM_T *)message;
    if (cfm->status != success)
        Panic();

    memcpy(&progRun->addr, &cfm->bd_addr, sizeof(bdaddr));
    DEBUG_LOG("ParamInit addr %04x:%02x:%06x", cfm->bd_addr.nap, cfm->bd_addr.uap, cfm->bd_addr.lap);
}
