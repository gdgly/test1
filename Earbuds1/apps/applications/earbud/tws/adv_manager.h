
#ifndef ADV_MANAGER_H
#define ADV_MANAGER_H

#define ADV_VOLUME_RANDOM_CODE_MASK 0X8000

// region 广播使能时，设置广播数据

uint8* advManagerAddManufacturerSpecificData(uint8 *ad_data, uint8* space);

// endregion

/// ble当前是否配对成功
bool appBleIsBond(void);

uint32 appBleGetBondCode(void);

struct BlePairInfo_T {
    uint8 btAddr[6];               // 手机经典蓝牙地址，需要与广播绑定码对应
    uint8 bleIsBond;
    uint16 advCode;
    uint32 bondCode;
};
typedef struct BlePairInfo_T BlePairInfo;

void appBleSetSync(bool status);

// region Task数据

/*
 * fun: 初始化广播数据
 */
void advManagerInit(void);

void advManagerPrepareAdvInfo(void);

// endregion

// region 特殊音量设置

// 能收到特殊音量时，说明经典蓝牙已经建立连接

/*
 * fun: 设置特殊音量，进行广播
 * todo 需要添加定时器，如果长时间没有处理，需要关闭BLE
 */
void advManagerSetSpecialVol(uint8 volSize);

/*
 * fun: 清楚特殊音量带来的操作
 * todo 在设置之后，如果没有连接，需要在蓝牙断开连接的时候清理，防止其他用户APP误连接
 */
void advManagerStopSpecialVol(void);

bool advManagerIsEnableSpecialVol(void);

// endregion

// region 广播功能 feature

enum {
    ADV_FEATURE_GAIA,
    ADV_FEATURE_PAIR,
    ADV_FEATURE_UPGRADE,
};

// endregion

// region 广播码

/*
 * fun: 选择对应的广播码
 *      配对时，使用固定的0X0000，其他情况，返回最后一次的配对信息
 * param: feature 广播的用途
 * param: advCode 具体的advCode
 * return: void
 */
void advManagerGetBleAdvInfo(uint8 *feature, uint16 *advCode);

/*
 * fun: 保存用户设置ble信息 前提：经典蓝牙已经建立连接
 * param: adv 广播码
 * param: bond 验证绑定码
 * param: timestamp 设置时的时间戳
 * return: 保存成功为TRUE，否则为FALSE
 */
bool advManagerSaveBleAdvInfo(uint16 adv, uint32 bond, uint32 timestamp);

// endregion

#endif
