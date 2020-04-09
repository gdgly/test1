#include "av_headset.h"
#include "av_headset_adv_manager.h"
#include "av_headset_adv_manager_private.h"
#include "tws/adv_manager.h"
#include "av_headset_log.h"

// region Param参数获取与设置

extern int16 ParamLoadBlePair(BlePairInfo *blePairInfo);
extern int16 ParamSaveBlePair(BlePairInfo *blePairInfo, uint32 timeModfy);

// endregion

// region Task数据

typedef struct  {
//    1	产品型号，固定为0X01
//    2	软件版本，固定为0X01
//    3	耳机位置
//    4	左耳机电量，FF表示异常
//    5	右耳机电量，FF表示异常
//    6	盒子电量，FF表示异常
//    7-8	随机连接码
//    9-17 left br/edr mac address
//    18-24 right br/edr mac address
//    25 ble feature
    uint8 product;
    uint8 version;
    uint8 position;
    uint8 leftPower;
    uint8 rightPower;
    uint8 casePower;
    uint8 randomCodeHigh;
    uint8 randomCodeLow;
    uint8 leftEarMac[6];
    uint8 rightEarMac[6];
    uint8 bleFeture;
} AdvManufacturerSpecificData;

uint8 bleFeature = 0XFF;
typedef struct {
    bool enableSpecialVol;  /// 是否使能特殊音量广播
    uint8 specialVolSize;   /// 特殊音量大小
    AdvManufacturerSpecificData advManufacturerSpecificData;  /// 广播使用的数据
} AdvManagerTaskData;

static AdvManagerTaskData advManagerTask;

void advManagerInit(void) {
    memset(&advManagerTask, 0X00, sizeof(AdvManagerTaskData));
    advManagerTask.advManufacturerSpecificData.product = 0X01;
    advManagerTask.advManufacturerSpecificData.version = 0X01;
}

static AdvManagerTaskData* advManagerGet(void) {
    return &advManagerTask;
}

extern void appGetLocalBrEdrAddress(uint8* addrbuf);
extern void appGetPeerBrEdrAddress(uint8* addrbuf);

void advManagerPrepareAdvInfo(void) {
    uint8 feature;
    uint16 advCode;
    advManagerGetBleAdvInfo(&feature, &advCode);
    advManagerSaveCurrentFeature(feature);

    AdvManagerTaskData *task = advManagerGet();
    AdvManufacturerSpecificData* sd = &(task->advManufacturerSpecificData);
    ProgRIPtr progRun = appSubGetProgRun();

    sd->position = appUIGetPositionInfo();
    appUIGetPowerInfo(progRun, &(sd->leftPower)); /// left - right - case

    sd->randomCodeHigh = (uint8)((advCode & 0XFF00) >> 8);
    sd->randomCodeLow = (uint8)(advCode & 0X00FF);

    if (appConfigIsLeft()) {
        appGetLocalBrEdrAddress(sd->leftEarMac);
        appGetPeerBrEdrAddress(sd->rightEarMac);
    } else {
        appGetPeerBrEdrAddress(sd->leftEarMac);
        appGetLocalBrEdrAddress(sd->rightEarMac);
    }

    sd->bleFeture = feature;
}

// endregion

// region 广播使能时，设置广播数据

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

uint8 *advManagerAddManufacturerSpecificData(uint8 *ad_data, uint8 *space) {
    AdvManagerTaskData* task = advManagerGet();
    advManagerPrepareAdvInfo();
    DEBUG_LOG("call advManagerAddManufacturerSpecificData for ble adv data");
    return addManufacturerSpecificData(ad_data, space, sizeof(AdvManufacturerSpecificData),
                                       (const uint8 *) &(task->advManufacturerSpecificData));
}

// endregion


// region ble配对信息

bool appBleIsBond(void) {
    BlePairInfo blePairInfo;
    ParamLoadBlePair(&blePairInfo);
    return blePairInfo.bleIsBond;
}

uint32 appBleGetBondCode(void) {
    BlePairInfo blePairInfo;
    ParamLoadBlePair(&blePairInfo);
    return blePairInfo.bondCode;
}

bool appBleFindBondCode(uint32 bindCode) {
    if (0X0000 == bindCode) {
        return FALSE;
    }

    BtAddrPrmPtr prm = &gBtAddrParam;
    for (int i = 0; i < BLEPAIR_COUNT; ++i) {
        if (prm->ble_pair[i].bondCode == bindCode) {
            return TRUE;
        }
    }

    return FALSE;
}

// endregion


void appBleSetSync(bool status) {
    (void)status;       // 这个函数在使用时间作为比较后，不需要单独保存了
}


// region 广播码

uint8 advManagerSelectFeature(void) {
    uint8 nowAdvFeature = 0XFF;
    if (appSmIsPairing()) {
        nowAdvFeature = ADV_FEATURE_PAIR;
    } else if (appDeviceIsHandsetHfpConnected() || appDeviceIsHandsetA2dpConnected() || appDeviceIsHandsetAvrcpConnected()) {
        nowAdvFeature = ADV_FEATURE_GAIA;
    } else if (appSmIsInDfuMode()) {
        nowAdvFeature = ADV_FEATURE_UPGRADE;
    }
    return nowAdvFeature;
}

static uint16 selectAdvCode(void) {
    uint16 nowAdvCode = 0XFFFF;
    AdvManagerTaskData* task = advManagerGet();
    if (appSmIsPairing()) {
        nowAdvCode = 0X0000;
    } else if (TRUE == task->enableSpecialVol) {
        nowAdvCode = ADV_VOLUME_RANDOM_CODE_MASK | task->specialVolSize;
    } else {
        BlePairInfo blePairInfo;
        ParamLoadBlePair(&blePairInfo);
        nowAdvCode = blePairInfo.advCode;
    }
    return nowAdvCode;
}

void advManagerGetBleAdvInfo(uint8 *feature, uint16 *advCode) {
    if (NULL != feature) {
        *feature = advManagerSelectFeature();
    }

    if (NULL != advCode) {
        *advCode = selectAdvCode();
    }
}

bool advManagerSaveBleAdvInfo(uint16 adv, uint32 bond, uint32 timestamp) {
    BlePairInfo blePairInfo;
    ParamLoadBlePair(&blePairInfo);
    blePairInfo.advCode = adv;
    blePairInfo.bondCode = bond;
    blePairInfo.bleIsBond = TRUE;
    ParamSaveBlePair(&blePairInfo, timestamp);
    return TRUE;
}

// endregion

// region 特殊音量设置

void advManagerSetSpecialVol(uint8 volSize) {
    AdvManagerTaskData* task = advManagerGet();
    task->enableSpecialVol = TRUE;
    task->specialVolSize = volSize;
}

void advManagerStopSpecialVol(void) {
    AdvManagerTaskData* task = advManagerGet();
    task->enableSpecialVol = FALSE;
    task->specialVolSize = 0X00;
}

bool advManagerIsEnableSpecialVol(void) {
    return advManagerGet()->enableSpecialVol;
}

void advManagerSaveCurrentFeature(uint8 feature) {
    bleFeature = feature;
}

uint8 advManagerGetBeforeFeature(void) {
    return bleFeature;
}

// endregion

// region 设置BLE地址

bool advManagerSetBleAddress(void) {
    DEBUG_LOG("advManagerSetBleAddress");

    bdaddr* bleAddress = NULL;
    bdaddr tempAddress;
    if (appConfigIsLeft()) {
        bleAddress = &gProgRunInfo.addr;
    } else {
        if (appDeviceGetPeerBdAddr(&tempAddress)) {
            bleAddress = &tempAddress;
        }
    }

    if (NULL == bleAddress) {
        DEBUG_LOG("advManagerSetBleAddress ble address is null, return false");
        return FALSE;
    }

    typed_bdaddr taddr;
    taddr.type = TYPED_BDADDR_RANDOM;
    taddr.addr.lap = bleAddress->lap;
    taddr.addr.uap = bleAddress->uap;
    taddr.addr.nap = bleAddress->nap;
    taddr.addr.nap = taddr.addr.nap | 0XC000;

    ConnectionDmBleConfigureLocalAddressAutoReq(ble_local_addr_write_static, &taddr, appConfigBleAddressChangeTime());

    DEBUG_LOG("advManagerSetBleAddress return true");
    return TRUE;
}

// endregion
