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

void subPhySetPosition( enum subPhyPosition pos) {
    subPhyTaskData* this = subPhyGetTaskData();
    this->position = pos;
}

bool subPhyIsCanNotifyCaseInfo(void) {
    subPhyTaskData* this = subPhyGetTaskData();
    return (this->position == SUB_PHY_POSITION_CASE_CLOSE || this->position == SUB_PHY_POSITION_CASE_OPEN);
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

    subPhySetPosition(SUB_PHY_POSITION_CASE_CLOSE);
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
    subPhySetPosition(SUB_PHY_POSITION_CASE_OPEN);
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
    subPhySetPosition(SUB_PHY_POSITION_IN_AIR);
}

void subPhyExitAir(void) {
    DEBUG_LOG("call subPhyExitAir");
    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
    subPhySetPosition(SUB_PHY_POSITION_CASE_OPEN);
}

void subPhyEnterEar(void) {
    DEBUG_LOG("call subPhyEnterEar");
    appUiPowerSave(POWER_MODE_IN_EAR);

    subPhySetPosition(SUB_PHY_POSITION_IN_EAR);
}

void subPhyExitEar(void) {
    DEBUG_LOG("call subPhyExitEar");
    appUiPowerSave(POWER_MODE_OUT_CASE);
    subPhySetPosition(SUB_PHY_POSITION_IN_AIR);
}
