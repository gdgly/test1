#ifndef PARAM_H
#define PARAM_H

#define  DEV_HWVER_LEN            (3)
#define  DEV_SWVER_LEN            (4)
#define  DEV_HWSWVER_LEN          (DEV_HWVER_LEN+1+DEV_SWVER_LEN)          // 版本信息总长度
#define  DEV_SN_LEN               (16)      // 耳机上使用16个字节的SN号

typedef enum {DEV_CASE=0, DEV_LEFT, DEV_RIGHT}DevType;         // 一套系统中三种分设备

//////////////////////////////////////////////////////////////////////////////////
/////        出厂固定不变的参数
//////////////////////////////////////////////////////////////////////////////////
typedef struct tagFIXPARAM {
    uint8          hw_ver[DEV_HWVER_LEN];                  // 硬件版本
    uint8          rev1;
    uint8          sn[DEV_SN_LEN];             // SN号，在生产的最后阶段写入
    int16          aud_adj;                    // 音频微调
    uint8          em20168_cal_already;         //接近光是否校准过了
    uint16         em20168_high_value;          //接近光校准高值
    uint16         em20168_low_value;           //接近光校准低值

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

    uint8          peerVer[DEV_HWSWVER_LEN];        // 对方耳机版本信息
    uint8          caseVer[DEV_HWSWVER_LEN];        // 盒子版本信息


    uint8          rev[8];
}BtAddrParam, *BtAddrPrmPtr;

extern BtAddrParam gBtAddrParam;
void BtAddrResetFactory(void);
void BtAddrParamDefault(void);
int16 ParamSaveBtAddrPrm(BtAddrPrmPtr pParam);
int16 ParamLoadBtAddrPrm(BtAddrPrmPtr pParam);
bool ParamUsingSingle(void);

/////////////////////////////////////////////////////////////////////////////////
//////// 一些用户可以修改的参数
/////////////////////////////////////////////////////////////////////////////////
typedef struct tagUSERPARAM {
    uint8          lKeyFunc, rKeyFunc;        // 耳机双击按键功能
    uint16         apolloEnable:1;            // 设置apollo是否使能0:不使能, 1:使能
    uint16         sensorEnable:1;            // 设置佩戴是否使能0:不使能, 1:使能

    uint8          rev[8];
}UserParam, *UserPrmPtr;

extern UserParam gUserParam;
void UserParamResetFactory(void);
void UserParamDefault(void);
int16 ParamSaveUserPrm(UserPrmPtr pParam);
int16 ParamLoadUserPrm(UserPrmPtr pParam);

int16 ParamLoadAll(void);
int16 ParamGetPeerAddr( typed_bdaddr *taddr);       // 从内存中获取
int16 ParamSavePeerAddr( typed_bdaddr *taddr);      // 盒子发来地址，需要保存
int16 ParamLoadBlePair( BlePairInfo *blePairInfo);
int16 ParamSaveBlePair(BlePairInfo *blePairInfo);

// 获取软硬件版本信息
// type: 0:盒子，1:LEFT， 2耳机(右）
int16 SystemGetVersion(DevType type, uint8 *buffer);
int16 SystemGetCurrentVersion(uint8 *buffer);             // Get Current Earbuds version
int16 SystemSetVersion(DevType type, uint8 *buffer);
int16 UserGetKeyFunc(uint8 *lKeyFunc, uint8 *rKeyFunc);   // 获取功能键
int16 UserSetKeyFunc(uint8 lKeyFunc, uint8 rKeyFunc);     // 设置功能键

void ParamConfigInit(void);
void ParamInitHandleClDmLocalBdAddrCfm(Message message);

// 组装测试使用
void box_send_test_cmd(uint8 *get_buf, uint8 *send_buf);
int16 ParamSaveSN(void *buff);
int16 ParamLoadSN(void *buff);
#endif // PARAM_H
