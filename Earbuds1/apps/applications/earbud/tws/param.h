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
    uint16         ucs146e0_calib;//ucs146e0校准底噪值

    uint8          lisdw12_cal_already;         //敲击是否校准过了
    int8           lisdw12_cal_x;//校准x轴值，下面相同
    int8           lisdw12_cal_y;
    int8           lisdw12_cal_z;

    uint8          rev[8];
}FixParam, *FixPrmPtr;

extern FixParam gFixParam;
void FixParamDefault(void);
int16 ParamSaveFixPrm(FixPrmPtr pParam);
int16 ParamLoadFixPrm(FixPrmPtr pParam);

#define BLEPAIR_COUNT         (4)      // 最多保存4组BLE配对码
/////////////////////////////////////////////////////////////////////////////////
/////// 蓝牙配对相关参数
/////////////////////////////////////////////////////////////////////////////////
typedef struct tagBTADDRPARAM {
    uint8          single_era;         // 1:单耳使用 0:TWS
    uint8          rev1[2];
    uint8          ble_pair_sync;      // BLE配对数据同步到对方耳机 0:不需要同步， 1：需要成功
    uint32         timeModfy;          // 最后一次改变本结构的时间
    BlePairInfo    ble_pair[BLEPAIR_COUNT];   // 保存ble配对信息adv广播，bind码
    typed_bdaddr   peer_addr;          // 保存双方配对耳机蓝牙地址

    uint8          peerVer[DEV_HWSWVER_LEN];        // 对方耳机版本信息;    废弃，不在使用
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
///
enum{TAP_PREVIOUS_TRACK = 0x01, TAP_NEXT_TRACK = 0x02,
     TAP_PLAY_PAUSE = 0x03, TAP_WACKUP = 0x04, TAP_SYSTEM = 0x05, TAP_DISABLED,
};



typedef struct tagUSERPARAM {
    uint8          lKeyFunc, rKeyFunc;        // 耳机双击按键功能
    uint16         apolloEnable:1;            // 设置apollo是否使能0:不使能, 1:使能
    uint16         sensorEnable:1;            // 设置佩戴是否使能0:不使能, 1:使能
    uint16         assistantType;             // 1:App 2:System
    uint32         assistantModifyTime;       // 助手修改时间
    uint32         sensorModifyTime;          // 佩戴检测修改时间
    uint8          caseSn[DEV_SN_LEN];        // 保存盒子的SN号用于显示

}UserParam, *UserPrmPtr;

extern UserParam gUserParam;
void UserParamResetFactory(void);
void UserParamDefault(void);
int16 ParamSaveUserPrm(UserPrmPtr pParam);
int16 ParamLoadUserPrm(UserPrmPtr pParam);

int16 ParamLoadAll(void);
int16 ParamGetPeerAddr( typed_bdaddr *taddr);       // 从内存中获取
int16 ParamSavePeerAddr( typed_bdaddr *taddr);      // 盒子发来地址，需要保存
int16 ParamLoadBlePair(BlePairInfo *blePairInfo);
BlePairInfo* ParamSearchBlePairByAddress(bdaddr* bd_addr);
int16 ParamSaveBlePair(BlePairInfo *blePairInfo, uint32 timeModfy);
int16 ParamSyncBlePair(int16 size_payload, uint8 *payload);    // 对方耳机发送过来的同步配对数据
int16 ParamSyncBlePairSucc(void); // 已经成功同步给对方耳机了
void bdaddr2buffer(bdaddr *addr, uint8* addrbuf);
void ParamPrintBlePair(void);

// 获取软硬件版本信息
// type: 0:盒子，1:LEFT， 2耳机(右）
int16 SystemGetVersion(DevType type, uint8 *buffer);
int16 SystemGetCurrentVersion(uint8 *buffer);             // Get Current Earbuds version
/// 比较当前耳机版本，与另一只耳机版本，当前耳机版本是否更低
/// Peer ? 0
/// Peer > Current 1
/// Peer = Current 2
/// Peer < Current 3
int SystemCheckMemoryVersion(void);
int16 SystemSetVersion(DevType type, uint8 *buffer);
int16 UserGetKeyFunc(uint8 *lKeyFunc, uint8 *rKeyFunc);   // 获取功能键
int16 UserSetKeyFunc(uint8 lKeyFunc, uint8 rKeyFunc);     // 设置功能键

void ParamConfigInit(void);
void ParamInitHandleClDmLocalBdAddrCfm(Message message);
int16 SystemParamSn(uint8 type, uint8 *sn, bool bSave);        // Load/Save SN

// 组装测试使用
void box_send_test_cmd(uint8 *get_buf, uint8 *send_buf);
int16 ParamSaveSN(void *buff);
int16 ParamLoadSN(void *buff);

const uint8* SystemGetCurrentSoftware(void);
#endif // PARAM_H
