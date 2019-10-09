#ifndef PARAM_H
#define PARAM_H

#define  DEV_HWVER_LEN            (3)
#define  DEV_SWVER_LEN            (4)
#define  DEV_HWSWVER_LEN          (DEV_HWVER_LEN+1+DEV_SWVER_LEN)          // 版本信息总长度

typedef enum {DEV_CASE=0, DEV_LEFT, DEV_RIGHT}DevType;         // 一套系统中三种分设备

//////////////////////////////////////////////////////////////////////////////////
/////        出厂固定不变的参数
//////////////////////////////////////////////////////////////////////////////////
typedef struct tagFIXPARAM {
    uint8          hw_ver[DEV_HWVER_LEN];                  // 硬件版本
    int16          aud_adj;                    // 音频微调

    uint8          rev[8];
}FixParam, *FixPrmPtr;

extern FixParam gFixParam;
void FixParamDefault(void);
int16 ParamSaveFixPrm(FixPrmPtr pParam);
int16 ParamLoadFixPrm(FixPrmPtr pParam);

/////////////////////////////////////////////////////////////////////////////////
/////// 蓝牙配对相关参数
/////////////////////////////////////////////////////////////////////////////////
typedef struct tagBTADDRPARAM {
    uint8          single_era;         // 1:单耳使用 0:TWS
    uint8          rev1[3];
    typed_bdaddr   peer_addr;          // 保存双方配对耳机蓝牙地址
    BlePairInfo    ble_pair;           // 保存ble配对信息adv广播，bind码

    uint8          peerVer[16];        // 对方耳机版本信息
    uint8          caseVer[16];        // 盒子版本信息


    uint8          rev[8];
}BtAddrParam, *BtAddrPrmPtr;

extern BtAddrParam gBtAddrParam;
void BtAddrParamDefault(void);
int16 ParamSaveBtAddrPrm(BtAddrPrmPtr pParam);
int16 ParamLoadBtAddrPrm(BtAddrPrmPtr pParam);
bool ParamUsingSingle(void);

/////////////////////////////////////////////////////////////////////////////////
//////// 一些用户可以修改的参数
/////////////////////////////////////////////////////////////////////////////////
typedef struct tagUSERPARAM {
    uint8          lKeyFunc, rKeyFunc;        // 耳机双击按键功能

    uint8          rev[8];
}UserParam, *UserPrmPtr;

extern UserParam gUserParam;
void UserParamDefault(void);
int16 ParamSaveUserPrm(UserPrmPtr pParam);
int16 ParamLoadUserPrm(UserPrmPtr pParam);

int16 ParamLoadAll(void);
int16 ParamGetPeerAddr( typed_bdaddr *taddr);       // 从内存中获取
int16 ParamLoadBlePair( BlePairInfo *blePairInfo);
int16 ParamSaveBlePair(BlePairInfo *blePairInfo);

// 获取软硬件版本信息
// type: 盒子，自身(左）， 耳机(右）
int16 SystemGetVersion(DevType type, uint8 *buffer);
int16 UserGetKeyFunc(uint8 *lKeyFunc, uint8 *rKeyFunc);   // 获取功能键
int16 UserSetKeyFunc(uint8 lKeyFunc, uint8 rKeyFunc);     // 设置功能键

void ParamConfigInit(void);
void ParamInitHandleClDmLocalBdAddrCfm(Message message);

#endif // PARAM_H
