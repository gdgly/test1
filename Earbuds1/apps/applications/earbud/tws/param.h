#ifndef PARAM_H
#define PARAM_H

//////////////////////////////////////////////////////////////////////////////////
/////        出厂固定不变的参数
//////////////////////////////////////////////////////////////////////////////////
typedef struct tagFIXPARAM {
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


#endif // PARAM_H
