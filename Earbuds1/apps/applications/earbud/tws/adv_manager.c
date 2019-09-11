#include "av_headset.h"
#include "av_headset_adv_manager.h"
#include "av_headset_adv_manager_private.h"
#include "tws/adv_manager.h"


extern int16 ParamLoadBlePair(BlePairInfo *blePairInfo);

extern int16 ParamSaveBlePair(BlePairInfo *blePairInfo);

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

uint8 *appAdvManagerAdvertdataAddManufacturerSpecificData(uint8 *ad_data, uint8 *space) {
    advTaskData.advManufacturerSpecificData.product = 0X01;
    advTaskData.advManufacturerSpecificData.version = 0X01;
    advTaskData.advManufacturerSpecificData.position = (0X40 | 0X08 | 0X01);
    advTaskData.advManufacturerSpecificData.rightPower = 0X80 | 0X33;
    advTaskData.advManufacturerSpecificData.leftPower = 0X80 | 0X34;
    advTaskData.advManufacturerSpecificData.casePower = 0X00 | 0X35;
//    advManufacturerSpecificData.randomCode = 0X66;

    return addManufacturerSpecificData(ad_data, space, sizeof(advTaskData.advManufacturerSpecificData),
                                       (const uint8 *) &advTaskData.advManufacturerSpecificData);
}

bool appAdvManagerAdvertdataUpdateRandomCode(uint16 randomCode) {
    uint16 beforeRandCode = advTaskData.advManufacturerSpecificData.randomCodeHigh;
    beforeRandCode = beforeRandCode << 8 + advTaskData.advManufacturerSpecificData.randomCodeLow;
    advTaskData.advManufacturerSpecificData.randomCodeHigh = (randomCode & 0XFF00) >> 8;
    advTaskData.advManufacturerSpecificData.randomCodeLow = (randomCode & 0X00FF);
    return (beforeRandCode == randomCode);
}

bool appBleIsBond(void) {
    return advTaskData.bleBondInfo.bleIsBond;
}

bool appAdvParamInit(void) {
    int16 res = ParamLoadBlePair(&advTaskData.bleBondInfo);
    if (res < 0) {
        advTaskData.bleBondInfo.bleIsBond = FALSE;
        advTaskData.bleBondInfo.advCode = 0X00;
        advTaskData.bleBondInfo.bondCode = 0X00;
    }
    advTaskData.advManufacturerSpecificData.randomCodeHigh = advTaskData.bleBondInfo.advCode & 0XFF00 >> 8;
    advTaskData.advManufacturerSpecificData.randomCodeLow = advTaskData.bleBondInfo.advCode & 0X00FF;

    return TRUE;
}


void appAdvParamSave(void) {
    int res = ParamSaveBlePair(&advTaskData.bleBondInfo);
    if (res < 0) {
        printf("save adv param failed\n");
    }
}

void appBleClearBond(void) {
    advTaskData.bleBondInfo.bleIsBond = FALSE;
    advTaskData.bleBondInfo.advCode = 0X00;
    advTaskData.bleBondInfo.bondCode = 0X00;
    appAdvParamSave();
    advTaskData.advManufacturerSpecificData.randomCodeHigh = 0X00;
    advTaskData.advManufacturerSpecificData.randomCodeLow = 0X00;
}

void appBleSetPond(uint16 advCode, uint32 bondCode) {
    advTaskData.bleBondInfo.bleIsBond = TRUE;
    printf("set pair ble code is : adv %04X, bond %04X\n", advCode, bondCode);
    advTaskData.bleBondInfo.advCode = advCode;
    advTaskData.bleBondInfo.bondCode = bondCode;
    appAdvParamSave();
    advTaskData.advManufacturerSpecificData.randomCodeHigh = (advTaskData.bleBondInfo.advCode & 0XFF00) >> 8;
    advTaskData.advManufacturerSpecificData.randomCodeLow = advTaskData.bleBondInfo.advCode & 0X00FF;
}

uint32 appBleGetBondCode(void) {
    return advTaskData.bleBondInfo.bondCode;
}
