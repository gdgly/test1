
#ifndef ADV_MANAGER_H
#define ADV_MANAGER_H

#define ADV_VOLUME_RANDOM_CODE_MASK 0X8000

bool appAdvParamInit(void);
void appAdvParamSave(uint32 timeModfy);

uint8* appAdvManagerAdvertdataAddManufacturerSpecificData(uint8 *ad_data, uint8* space);
bool appAdvManagerAdvertdataUpdateRandomCode(uint16 randomCode);

void appPrivateBleSetRandomCode(uint16 advCode);


/// ble当前是否配对成功
bool appBleIsBond(void);

struct BlePairInfo_T {
    uint8 btAddr[6];               // 手机经典蓝牙地址，需要与广播绑定码对应
    uint8 bleIsBond;
    uint16 advCode;
    uint32 bondCode;
};
typedef struct BlePairInfo_T BlePairInfo;

void appBleClearBond(void);
void appBleSetBond(uint16 advCode, uint32 bondCode, uint32 timeModfy);
void appBleSetSync(bool status);
uint32 appBleGetBondCode(void);

enum {
    ADV_FETURE_GAIA,
    ADV_FETURE_PAIR,
    ADV_FETURE_UPGRADE,
};

void appBleAdvFeture(uint8 feture);
void appBleSelectFeture(void);
#endif
