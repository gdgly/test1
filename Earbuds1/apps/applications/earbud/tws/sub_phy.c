#include <av_headset_phy_state.h>
#include <av_headset.h>
#include "tws/sub_phy.h"
#include "logging.h"
#include "tws/peer.h"



subPhyTaskData theSubPhyTaskData;

void subPhyTaskInit(void) {
    memset(&theSubPhyTaskData, 0x00, sizeof(subPhyTaskData));
}

subPhyTaskData* subPhyGetTaskData(void) {
    return &theSubPhyTaskData;
}

void subPhySetVirtualPosition(enum subPhyPosition pos) {
    subPhyTaskData* this = subPhyGetTaskData();
    DEBUG_LOG("subPhySetVirtualPosition before:%d new:%d", this->virtualPosition, pos);
    UartPuts2x("VP state:", this->virtualPosition, pos);
    this->virtualPosition = pos;
}

bool subPhyIsCanNotifyCaseInfo(void) {
    subPhyTaskData* this = subPhyGetTaskData();
    return subPhyVirtualStateIsCanConnectCase(this->virtualPosition);
}

enum subPhyPosition subPhyGetVirtualPosition(void) {
    subPhyTaskData* this = subPhyGetTaskData();
    return this->virtualPosition;
}

void subPhyEnterCase(void) {
    DEBUG_LOG("call subPhyEnterCase");
    DEBUG_LOG("parse APP_ATTACH_PLC_IN event");
    online_dbg_record(ONLINE_DBG_IN_CASE);

    phyStateTaskData* phy_state = appGetPhyState();
    MessageSend(&phy_state->task, PHY_STATE_INTERNAL_IN_CASE_EVENT, NULL);
//    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
//    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
//    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_IN, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_CASE_CLOSE);
}

void subPhyExitCase(void) {
    DEBUG_LOG("call subPhyExitCase");
    online_dbg_record(ONLINE_DBG_OUT_CASE);

    /// 实际操作，用户打开充电盒，没有取出耳机
    phyStateTaskData* phy_state = appGetPhyState();
    phy_state->in_proximity = FALSE;
    MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT);
    MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT, NULL, 50);

//    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_IN);
//    MessageCancelAll(appGetUiTask(), APP_ATTACH_PLC_OUT);
//    MessageSendLater(appGetUiTask(), APP_ATTACH_PLC_OUT, NULL, 50);

    appUiPowerSave(POWER_MODE_IN_CASE_OPEN);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_CASE_OPEN);

}

void subPhyEnterAir(void) {
    DEBUG_LOG("call subPhyEnterAir");
    if (appSmIsInDfuMode()) {
        DEBUG_LOG("subPhyEnterAir current is in dfu, need exit dfu, to enter normal model");
//        phyStateTaskData* phy_state = appGetPhyState();
//        MessageCancelAll(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT);
//        MessageSendLater(&phy_state->task, PHY_STATE_INTERNAL_OUT_OF_CASE_EVENT, NULL, 50);
        appPeerSigTxUpgradeExitReq(appGetGaiaTask());
    }

    appUiPowerSave(POWER_MODE_OUT_CASE);
    subPhySetVirtualPosition(SUB_PHY_POSITION_IN_AIR);

    appPeerSyncSend(FALSE);
    MessageSend(appGetUiTask(), APP_NOTIFY_DEVICE_CON_POS, NULL);
}

void subPhyExitAir(void) {
    DEBUG_LOG("call subPhyExitAir");
    if (FALSE == appGetCaseIsOpen()) {
        DEBUG_LOG("subPhyExitAir: now case is close, so don't send message to application");
        return;
    }
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

bool subPhyVirtualStateIsCanConnectCase(uint8 status) {
    return (status == SUB_PHY_POSITION_IN_CASE_CLOSE || status == SUB_PHY_POSITION_IN_CASE_OPEN);
}

bool subPhyCurrentVirtualStateIsCanConnectCase(void) {
    uint8 st = subPhyGetVirtualPosition();
    return subPhyVirtualStateIsCanConnectCase(st);
}


