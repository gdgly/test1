
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
#include "av_headset_gaia_starot.h"

#define PSKEY_CUSTOMER0_VALUE   (200)                         // ID, from trap_api/ps.h

#define PSKEY_PEER_ADDR         (PSKEY_CUSTOMER0_VALUE)       // 保存双方配对耳机蓝牙地址
#define PSKEY_BLE_PAIR          (PSKEY_CUSTOMER0_VALUE + 1)       // 保存ble配对信息adv广播，bind码

#define PSKEY_FIX_PARAM         (PSKEY_CUSTOMER0_VALUE)       // 固定参数
#define PSKEY_BTADDR_PARAM      (PSKEY_CUSTOMER0_VALUE+2)     // 蓝牙相关参数
#define PSKEY_USER_PARAM        (PSKEY_CUSTOMER0_VALUE+4)     // 用户设置参数



/* SW版本共4字节： 01.01.23.xx   xx为测试使用*/
const uint8 SYSTEM_SW_VERSION[DEV_SWVER_LEN] = {0x01,0x02,0x3c,0x01};   // 每次发布版本修改这儿的值

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
    prm->hw_ver[0]    = 0x00;
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


int16 ParamSaveSN(void *buff)
{
    memcpy(gFixParam.sn, buff, DEV_SN_LEN);

    ParamSaveFixPrm(NULL);
    return 0;
}

int16 ParamLoadSN(void *buff)
{
    memcpy(buff, gFixParam.sn, DEV_SN_LEN);
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/////// 蓝牙配对相关参数
/////////////////////////////////////////////////////////////////////////////////
BtAddrParam gBtAddrParam;

void BtAddrResetFactory(void) {
    BtAddrPrmPtr prm = &gBtAddrParam;

    uint8 se = prm->single_era;
    typed_bdaddr pa;
    memcpy(&pa, &(prm->peer_addr), sizeof(typed_bdaddr));

    BtAddrParamDefault();

    prm->single_era = se;
    memcpy(&(prm->peer_addr), &pa, sizeof(typed_bdaddr));

    ParamSaveBtAddrPrm(NULL);
}

void BtAddrParamDefault(void)
{
    BtAddrPrmPtr prm = &gBtAddrParam;

    memset(prm, 0, sizeof(BtAddrParam));
    prm->single_era      = FALSE;

    // 默认ble配对码和双击配置，已经同步至另一只耳机
    // todo 后期如果有要同步的消息标识位，需要在这里添加

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
UserPrmPtr GetUserParam(void);
UserPrmPtr GetUserParam(void)
{
    return &gUserParam;
}

void UserParamDefault(void)
{
    UserPrmPtr prm = &gUserParam;

    memset(prm, 0, sizeof(UserParam));
    prm->lKeyFunc = 0X02;
    prm->rKeyFunc = 0X03;
    prm->apolloEnable = FALSE;
    prm->sensorEnable = TRUE;
    prm->assistantType = ASSISTANT_TYPE_APP;
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

// 检查是否为独立使用，没有耳机配对
// 单耳独立使用 0:daul 1:mon  ff:unknown
bool ParamUsingSingle(void)
{
    return (bool)gBtAddrParam.single_era;
}

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

int16 ParamSavePeerAddr( typed_bdaddr *taddr)
{
    if(memcmp(&gBtAddrParam.peer_addr, taddr, sizeof(typed_bdaddr)) == 0)
        return 0;    // Same Addr

    if(taddr->addr.nap == 0xFFFF && taddr->addr.uap == 0xFF && taddr->addr.lap == 0xFFFFFF)
        gBtAddrParam.single_era = TRUE;
    else
        gBtAddrParam.single_era = FALSE;

    memcpy(&gBtAddrParam.peer_addr, taddr, sizeof(typed_bdaddr));

    return ParamSaveBtAddrPrm(&gBtAddrParam);
}

void bdaddr2buffer(bdaddr *addr, uint8* addrbuf)
{
    addrbuf[0] = (addr->nap >> 8) & 0xFF;
    addrbuf[1] = (addr->nap & 0xFF);
    addrbuf[2] = addr->uap;
    addrbuf[3] = (addr->lap >> 16) & 0xFF;
    addrbuf[4] = (addr->lap >> 8) & 0xFF;
    addrbuf[5] = (addr->lap & 0xFF);
}

// ret: <0 没找到 (根据蓝牙地址来查找）
//      >=0 对应的序号
static int16 ParamSearchBlePair(BlePairInfo *pairArr, uint8 *btAddr)
{
    int i, ret = -1;

    for(i = 0; i < BLEPAIR_COUNT; i++) {
        if(memcmp(pairArr[i].btAddr, btAddr, 6) != 0)
            continue;

        ret = i;
        break;
    }

    return ret;
}

// 将所有的配对码后移一个序号
static int16 ParamMoveBlePair(BlePairInfo *pairArr, uint8 endNo)
{
    int i;

    if(endNo < 0 || endNo >= BLEPAIR_COUNT)
        endNo = BLEPAIR_COUNT - 1;

    for(i = endNo; i > 0; i--) {
        memcpy(&pairArr[i], &pairArr[i-1], sizeof(BlePairInfo));
    }

    return 0;
}


// 这儿能保存多组配对码，需要将最后使用的给保存在第一个位置上，其它后移
int16 ParamSaveBlePair(BlePairInfo *blePairInfo, uint32 timeModfy)
{
    int iFind = -1;
    BtAddrPrmPtr prm = &gBtAddrParam;
    deviceTaskData *theDevice = appGetDevice();

    if(theDevice->handset_connected && !BdaddrIsZero(&theDevice->handset_bd_addr)) {
        bdaddr2buffer(&theDevice->handset_bd_addr, blePairInfo->btAddr);
    }

    iFind = ParamSearchBlePair(prm->ble_pair, blePairInfo->btAddr);
    ParamMoveBlePair(prm->ble_pair, iFind);
    DEBUG_LOG("SAVE Pair iFind=%d handset=%d adv=%x bond=%x", iFind, theDevice->handset_connected,
        blePairInfo->advCode, blePairInfo->bondCode);

    // 保存在第一个节点上
    prm->ble_pair_sync = 1;
    memcpy(&prm->ble_pair[0], blePairInfo, sizeof(BlePairInfo));

    prm->timeModfy  = timeModfy;
    return ParamSaveBtAddrPrm(prm);
}

// 已经成功同步给对方耳机了
int16 ParamSyncBlePairSucc(void)
{
    BtAddrPrmPtr prm = &gBtAddrParam;

    if(0 == prm->ble_pair_sync) {
        return 0;
    }
    prm->ble_pair_sync = 0;
    return ParamSaveBtAddrPrm(prm);
}


void ParamPrintBlePair(void) {
    BtAddrPrmPtr prm = &gBtAddrParam;

    for(int i = 0; i < BLEPAIR_COUNT; i++) {
        BlePairInfo* p = &(prm->ble_pair[i]);
        DEBUG_LOG("ParamPrintBlePair  %d: mac[%02X:%02X:%02X:%02X:%02X:%02X] adv[%04X] bond[%08X] bleIsBond[%02X]",
                i, p->btAddr[0], p->btAddr[1],p->btAddr[2],p->btAddr[3],p->btAddr[4],p->btAddr[5],
                p->advCode, p->bondCode, p->bleIsBond);
    }
}

// 主耳机同步数配对码以副耳机
// payload : BtAddrParam.syncTime 开始
int16 ParamSyncBlePair(int16 size_payload, uint8 *payload)
{
    uint32 timeModfy;
    BtAddrPrmPtr prm = &gBtAddrParam;

    if(size_payload < (4+sizeof(BlePairInfo))) {
        DEBUG_LOG("Error SysncBlePair");
        return -1;
    }

    memcpy(&timeModfy, payload, 4);
    if(timeModfy > prm->timeModfy) {
        prm->ble_pair_sync = 0;
        memcpy(&prm->timeModfy, payload, size_payload);
        ParamSaveBtAddrPrm(prm);
    }
    else
        timeModfy = 0;

    DEBUG_LOG("SynclePair, timeNow=%d", timeModfy);
    return 0;
}

int16 ParamLoadBlePair( BlePairInfo *blePairInfo)
{
    int iNo = 0;
    BtAddrPrmPtr prm = &gBtAddrParam;
#if 0
    deviceTaskData *theDevice = appGetDevice();

    // 检查当前经典蓝牙是否已经连接,没有连接返回第一组
    if(theDevice->handset_connected && !BdaddrIsZero(&theDevice->handset_bd_addr)) {
        bdaddr2buffer(&theDevice->handset_bd_addr, blePairInfo->btAddr);
        iNo = ParamSearchBlePair(prm->ble_pair, blePairInfo->btAddr);
        if(iNo >= 0 && iNo < BLEPAIR_COUNT) {   // 返回对应蓝牙地址的那一组
            memcpy(blePairInfo, &prm->ble_pair[iNo], sizeof(BlePairInfo));
            goto out;
        }
    }
    else {         // 查找最后一次连接的手机地址
        bdaddr bd_addr;
        appDeviceAttributes attributes;
        if(appDeviceGetAttributes(&bd_addr, DEVICE_TYPE_HANDSET, &attributes, NULL) == TRUE) {
            bdaddr2buffer(&bd_addr, blePairInfo->btAddr);
            iNo = ParamSearchBlePair(prm->ble_pair, blePairInfo->btAddr);
            if(iNo >= 0 && iNo < BLEPAIR_COUNT) {   // 返回对应蓝牙地址的那一组
                memcpy(blePairInfo, &prm->ble_pair[iNo], sizeof(BlePairInfo));
                goto out;
            }
        }
    }
#else
    bdaddr bd_addr;
    if(appDeviceGetHandsetBdAddr(&bd_addr) == TRUE) {
        bdaddr2buffer(&bd_addr, blePairInfo->btAddr);
        iNo = ParamSearchBlePair(prm->ble_pair, blePairInfo->btAddr);
        if(iNo >= 0 && iNo < BLEPAIR_COUNT) {   // 返回对应蓝牙地址的那一组
            memcpy(blePairInfo, &prm->ble_pair[iNo], sizeof(BlePairInfo));
            DEBUG_LOG("LoadBlePair: adv=%x bond=%x", blePairInfo->advCode, blePairInfo->bondCode);
            goto out;
        }
    }
#endif

    // 返回全0
    memset(blePairInfo, 0x00, sizeof(BlePairInfo));
out:
    return sizeof(BlePairInfo);
}

BlePairInfo* ParamSearchBlePairByAddress(bdaddr* bd_addr) {
    if (NULL == bd_addr) {
        return NULL;
    }
    uint8 addressBuffer[6];
    bdaddr2buffer(bd_addr, addressBuffer);
    BtAddrPrmPtr prm = &gBtAddrParam;
    int iNo = ParamSearchBlePair(prm->ble_pair, addressBuffer);
    if(iNo >= 0 && iNo < BLEPAIR_COUNT) {   // 返回对应蓝牙地址的那一组
        return &(prm->ble_pair[iNo]);
    }
    return NULL;
}

// 获取软硬件版本信息
// hwVer[3]+rev[1]+swVer[4]
// type: 0：盒子， 1：LEFT，2：RIGTH
int16 SystemGetVersion(DevType type, uint8 *buffer)
{
    uint8 *ptr = (uint8*)buffer;

    switch(type) {
    case DEV_CASE:
        memcpy(ptr, gBtAddrParam.caseVer, DEV_HWSWVER_LEN);
        break;
    case DEV_LEFT:
        if(appConfigIsLeft()) {
            memcpy(ptr, appCurrVersionGet(), DEV_HWSWVER_LEN);
        } else {
            memcpy(ptr, appPeerVersionGet(), DEV_HWSWVER_LEN);
        }
        break;
    case DEV_RIGHT:
        if(appConfigIsLeft()) {
            memcpy(ptr, appPeerVersionGet(), DEV_HWSWVER_LEN);
        } else {
            memcpy(ptr, appCurrVersionGet(), DEV_HWSWVER_LEN);
        }
        break;
    }

    return DEV_HWSWVER_LEN;       // BYTE
}

int16 SystemGetCurrentVersion(uint8 *buffer)             // Get Current Earbuds version
{
    return SystemGetVersion(appConfigIsLeft() ? DEV_LEFT : DEV_RIGHT, buffer);
}

/// 比较当前耳机版本，与另一只耳机版本，当前耳机版本是否更低

extern void appPeerSigTxSyncVersionReq(Task task, bool needFocusReplayVersion);

int SystemCheckMemoryVersion(void) {
    int unkonw = 0;

    if (appPeerVersionIsEmpty()) {
        ///没有对方地址信息，情况不明，认为对方版本更低(保守策略)
        appPeerSigTxSyncVersionReq(appGetUiTask(), TRUE);
        return unkonw;
    }

    int peerLast = 1, sameLast = 2, currentLast = 3;
    uint8* current = gProgRunInfo.currVer;
    uint8* peer = gProgRunInfo.peerVer;
    /// 只比较软件版本，不比较硬件版本
    for (int i = DEV_HWVER_LEN + 1; i < 8; ++i) {
        if (current[i] < peer[i]) {
            return  peerLast;
        } else if (current[i] > peer[i]) {
            return currentLast;
        }
    }
    return sameLast;
}

int16 SystemSetVersion(DevType type, uint8 *buffer)
{
    uint8 *ptr = (uint8*)buffer;
    switch(type) {
    case DEV_CASE:
        memcpy(gBtAddrParam.caseVer, ptr, DEV_HWSWVER_LEN);
        break;
    case DEV_LEFT:   // 设备自身的版本号不能被修改
        if(appConfigIsRight())
            memcpy(gBtAddrParam.peerVer, ptr, DEV_HWSWVER_LEN);
        break;
    case DEV_RIGHT:
        if(appConfigIsLeft())
            memcpy(gBtAddrParam.peerVer, ptr, DEV_HWSWVER_LEN);
        break;
    }
    return DEV_HWSWVER_LEN;

}

// type: 0=本机SN 1=盒子SN
int16 SystemParamSn(uint8 type, uint8 *sn, bool bSave)        // Load/Save SN
{
    if(0 == type) {
        FixPrmPtr prm = &gFixParam;
        if(bSave) {
            if(memcmp(prm->sn, sn, DEV_SN_LEN) == 0)
                return 0;

            memcpy(prm->sn, sn, DEV_SN_LEN);
            ParamSaveFixPrm(prm);
        }
        else {
            memcpy(sn, prm->sn, DEV_SN_LEN);
        }
    }
    else {
        UserPrmPtr prm = GetUserParam();
        if(bSave) {
            if(memcmp(prm->caseSn, sn, DEV_SN_LEN) == 0)
                return 0;

            memcpy(prm->caseSn, sn, DEV_SN_LEN);
            ParamSaveUserPrm(prm);
        }
        else
            memcpy(sn, prm->caseSn, DEV_SN_LEN);
    }
    return 0;
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


extern void advManagerInit(void);

void ParamConfigInit(void)
{
    ParamLoadAll();

    /* Get local device address */
    ConnectionReadLocalAddr(appGetAppTask());

    /* 广播数据 */
    advManagerInit();
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

void UserParamResetFactory(void) {
    uint8 electric_quantity = gUserParam.electricQuantity;
    UserParamDefault();
    gUserParam.electricQuantity = electric_quantity;
    ParamSaveUserPrm(NULL);
}


const uint8 *SystemGetCurrentSoftware(void) {
    return SYSTEM_SW_VERSION;
}
