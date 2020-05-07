#include <av_headset_phy_state.h>
#include <av_headset.h>
#include "tws/sub_phy.h"
#include "logging.h"

void subPhyEnterCase(void) {
    DEBUG_LOG("call subPhyEnterCase");

    phyStateTaskData* phy_state = appGetPhyState();
    MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_IN_CASE_EVENT);
    MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_IN_CASE_EVENT, NULL, 50);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_IN, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE);
}

void subPhyExitCase(void) {
    DEBUG_LOG("call subPhyExitCase");
    // todo 临时注释，重新调试低功耗
//    if (FALSE == appGetCaseIsOpen()) {
//        if(_ear_en_dormant) {
//            _ear_en_dormant = 0;
//            MessageSendLater(appGetUiTask(), APP_UI_ENTER_DORMANT, NULL, D_SEC(1));
//        }
//        DEBUG_LOG("max20340_notify_plc_out, now case is close, so don't send message to application");
//        return;
//    }
    phyStateTaskData* phy_state = appGetPhyState();
    MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT);
    MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT, NULL, 50);

    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_OUT, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
}

void subPhyEnterAir(void) {
    DEBUG_LOG("call subPhyEnterAir");

    appUiPowerSave(POWER_MODE_OUT_CASE);
}

void subPhyExitAir(void) {
    DEBUG_LOG("call subPhyExitAir");
    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
}

void subPhyEnterEar(void) {
    DEBUG_LOG("call subPhyEnterEar");
    appUiPowerSave(POWER_MODE_IN_EAR);
}

void subPhyExitEar(void) {
    DEBUG_LOG("call subPhyExitEar");
    appUiPowerSave(POWER_MODE_OUT_CASE);
}
