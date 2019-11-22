
#ifndef ADV_MANAGER_H
#define ADV_MANAGER_H

#define ADV_VOLUME_RANDOM_CODE_MASK 0X8000

bool appAdvParamInit(void);
void appAdvParamSave(void);

uint8* appAdvManagerAdvertdataAddManufacturerSpecificData(uint8 *ad_data, uint8* space);
bool appAdvManagerAdvertdataUpdateRandomCode(uint16 randomCode);

void appPrivateBleSetRandomCode(uint16 advCode);


/// ble当前是否配对成功
bool appBleIsBond(void);

struct BlePairInfo_T {
    uint8 bleIsBond;
    uint8 bleIsSync;
    uint16 advCode;
    uint32 bondCode;
};
typedef struct BlePairInfo_T BlePairInfo;

void appBleClearBond(void);
void appBleSetBond(uint16 advCode, uint32 bondCode);
void appBleSetSync(bool status);
uint32 appBleGetBondCode(void);

#endif
