#include <av_headset_phy_state.h>
#include <av_headset.h>
#include "tws/sub_phy.h"
#include "logging.h"



subPhyTaskData theSubPhyTaskData;

void subPhyTaskInit(void) {
    memset(&theSubPhyTaskData, 0x00, sizeof(subPhyTaskData));
}

subPhyTaskData* subPhyGetTaskData(void) {
    return &theSubPhyTaskData;
}

void subPhySetVirtualPosition(enum subPhyPosition pos) {
    subPhyTaskData* this = subPhyGetTaskData();
    this->virtualPosition = pos;
}

bool subPhyIsCanNotifyCaseInfo(void) {
    subPhyTaskData* this = subPhyGetTaskData();
    return (this->virtualPosition == SUB_PHY_POSITION_IN_CASE_CLOSE || this->virtualPosition == SUB_PHY_POSITION_IN_CASE_OPEN);
}

enum subPhyPosition subPhyGetVirtualPosition(void) {
    subPhyTaskData* this = subPhyGetTaskData();
    return this->virtualPosition;
}

void subPhyEnterCase(void) {
    DEBUG_LOG("call subPhyEnterCase");

    phyStateTaskData* phy_state = appGetPhyState();
    MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_IN_CASE_EVENT);
    MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_IN_CASE_EVENT, NULL, 50);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_IN, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE);

    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_CASE_CLOSE);
}

void subPhyExitCase(void) {
    DEBUG_LOG("call subPhyExitCase");

    phyStateTaskData* phy_state = appGetPhyState();
    MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT);
    MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT, NULL, 50);

    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_OUT, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_CASE_OPEN);
}

void subPhyEnterAir(void) {
    DEBUG_LOG("call subPhyEnterAir");

    if (FALSE == appGetCaseIsOpen()) {
        // todo 临时注释，重新调试低功耗
//        if(_ear_en_dormant) {
//            _ear_en_dormant = 0;
//            MessageSendLater(appGetUiTask(), APP_UI_ENTER_DORMANT, NULL, D_SEC(1));
//        }
        DEBUG_LOG("subPhyEnterAir: now case is close, so don't send message to application");
        return;
    }

    appUiPowerSave(POWER_MODE_OUT_CASE);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_AIR);

    appPeerSyncSend(FALSE);
    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
}

void subPhyExitAir(void) {
    DEBUG_LOG("call subPhyExitAir");
    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_CASE_OPEN);
    appPeerSyncSend(FALSE);
    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
}

void subPhyEnterEar(void) {
    DEBUG_LOG("call subPhyEnterEar");
    appUiPowerSave(POWER_MODE_IN_EAR);

    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_EAR);
}

void subPhyExitEar(void) {
    DEBUG_LOG("call subPhyExitEar");
    appUiPowerSave(POWER_MODE_OUT_CASE);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_AIR);
}


