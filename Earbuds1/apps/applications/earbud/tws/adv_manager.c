#include "av_headset.h"
#include "av_headset_adv_manager.h"
#include "av_headset_adv_manager_private.h"
#include "tws/adv_manager.h"
#include "av_headset_log.h"


extern int16 ParamLoadBlePair(BlePairInfo *blePairInfo);

extern int16 ParamSaveBlePair(BlePairInfo *blePairInfo);

void appAdvManagerAdvertdatafunc(void);

//static void appPrivateBleSetRandomCode(uint16 advCode);

static uint8 *addManufacturerSpecificData(uint8 *ad_data, uint8 *space, uint16 size_specific_data, const uint8 *specific_data) {
    uint8 field_length;
    uint8 data_length = size_specific_data;
    uint8 ad_tag = ble_ad_type_manufacturer_specific_data;
    uint8 usable_space = USABLE_SPACE(space);

    if ((data_length == 0) || usable_space <= 1 || data_length > usable_space) {
        return ad_data;
    }

    field_length = AD_FIELD_LENGTH(data_length);
    WRITE_AD_DATA(ad_data, space, field_length);
    WRITE_AD_DATA(ad_data, space, ad_tag);

    memmove(ad_data, specific_data, data_length);
    ad_data += data_length;
    *space -= data_length;

    return ad_data;
}

struct AdvManufacturerSpecificData {
//    1	产品型号，固定为0X01
//    2	软件版本，固定为0X01
//    3	耳机位置
//    4	左耳机电量，FF表示异常
//    5	右耳机电量，FF表示异常
//    6	盒子电量，FF表示异常
//    7	随机连接码
    uint8 product;
    uint8 version;
    uint8 position;
    uint8 leftPower;
    uint8 rightPower;
    uint8 casePower;
    uint8 randomCodeHigh;
    uint8 randomCodeLow;
};

struct AdvTaskData {
    struct AdvManufacturerSpecificData advManufacturerSpecificData;
    BlePairInfo bleBondInfo;
};

static struct AdvTaskData advTaskData;

void appAdvManagerAdvertdatafunc(void){
    ProgRIPtr  progRun = appSubGetProgRun();
    advTaskData.advManufacturerSpecificData.position = (0X40 | 0X08 | 0X01);
    advTaskData.advManufacturerSpecificData.rightPower = progRun->peerElectrity;
    advTaskData.advManufacturerSpecificData.leftPower = progRun->iElectrity;
    advTaskData.advManufacturerSpecificData.casePower = progRun->caseElectrity;
}

uint8 *appAdvManagerAdvertdataAddManufacturerSpecificData(uint8 *ad_data, uint8 *space) {
    advTaskData.advManufacturerSpecificData.product = 0X01;
    advTaskData.advManufacturerSpecificData.version = 0X01;

    appAdvManagerAdvertdatafunc();
//    advManufacturerSpecificData.randomCode = 0X66;

    DEBUG_LOG("call appAdvManagerAdvertdataAddManufacturerSpecificData for ble adv data");
    return addManufacturerSpecificData(ad_data, space, sizeof(advTaskData.advManufacturerSpecificData),
                                       (const uint8 *) &advTaskData.advManufacturerSpecificData);
}

bool appAdvManagerAdvertdataUpdateRandomCode(uint16 randomCode) {
    uint16 beforeRandCode = advTaskData.advManufacturerSpecificData.randomCodeHigh;
    beforeRandCode = (beforeRandCode << 8) + advTaskData.advManufacturerSpecificData.randomCodeLow;
    appPrivateBleSetRandomCode(randomCode);
    return (beforeRandCode == randomCode);
}

bool appBleIsBond(void) {
    return advTaskData.bleBondInfo.bleIsBond;
}

bool appAdvParamInit(void) {
    ParamLoadBlePair(&advTaskData.bleBondInfo);
    appPrivateBleSetRandomCode(advTaskData.bleBondInfo.advCode);
    return TRUE;
}

void appAdvParamSave(void) {
    int res = ParamSaveBlePair(&advTaskData.bleBondInfo);
    if (res < 0) {
        printf("save adv param failed\n");
    }
}

void appBleClearBond(void) {
    DEBUG_LOG("call appBleClearBond");
    advTaskData.bleBondInfo.bleIsBond = FALSE;
    advTaskData.bleBondInfo.advCode = 0X00;
    advTaskData.bleBondInfo.bondCode = 0X00;
    appAdvParamSave();
    appPrivateBleSetRandomCode(0X00);
}

void appBleSetBond(uint16 advCode, uint32 bondCode) {
    DEBUG_LOG("set pair ble code is : adv %04X, bond %04X", advCode, bondCode);
    advTaskData.bleBondInfo.bleIsBond = TRUE;
    advTaskData.bleBondInfo.advCode = advCode;
    advTaskData.bleBondInfo.bondCode = bondCode;
    appAdvParamSave();
    appPrivateBleSetRandomCode(advCode);
}

uint32 appBleGetBondCode(void) {
    return advTaskData.bleBondInfo.bondCode;
}

void appPrivateBleSetRandomCode(uint16 advCode) {
    advTaskData.advManufacturerSpecificData.randomCodeHigh = (advCode & 0XFF00) >> 8;
    advTaskData.advManufacturerSpecificData.randomCodeLow = advCode & 0X00FF;
    DEBUG_LOG("random code : high %02X, low %02X", advTaskData.advManufacturerSpecificData.randomCodeHigh,
              advTaskData.advManufacturerSpecificData.randomCodeLow);
}
