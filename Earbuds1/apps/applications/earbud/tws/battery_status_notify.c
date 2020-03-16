//
// Created by Starot on 2020/3/16.
//
#include "tws/battery_status_notify.h"

#include <string.h>
#include <byte_utils.h>
#include <stdlib.h>
#include "av_headset.h"
#include "av_headset_latency.h"
#include "av_headset_power.h"
#include "av_headset_log.h"

static const char batt_enable_string[] = "AT+XAPL=05AC-1702-0100,7\r";
static const char batt_level_string[] = "AT+IPHONEACCEV=1,1,0\r";

#define BATTERY_POS   19

// region 数据定义

typedef struct {
    TaskData task;
    batteryRegistrationForm Client;
} BatteryStatusTaskData;

BatteryStatusTaskData batteryStatusTaskData;

// endregion

static void sendBatteryByAtCmd(uint8 percent) {
    UNUSED(percent);
    char *data = (char*)PanicUnlessMalloc(sizeof(batt_level_string));
    if (NULL != data) {
        memcpy(data, batt_level_string, sizeof(batt_level_string));
        DEBUG_LOGF("AT: Send Cmd battery percent %d\n", percent);
        data[BATTERY_POS] = (percent / 10) + 0x30;
        DEBUG_PRINTF("AT Send:[%s]\n", data);
//        HfpAtCmdRequest(hfp_primary_link, data);
        free(data);
    }
}

static void batteryLevelUpdatePercent(MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T *msg) {
    DEBUG_LOG("AT: appAtCmdHandleBatteryLevelUpdatePercent\n");
    if (appDeviceIsHandsetHfpConnected()) {
        sendBatteryByAtCmd(msg->percent);
    }
}

static void batteryStatusNotifyHandleMessage(Task task, MessageId id, Message message) {
    UNUSED(task);
    UNUSED(message);
    switch (id) {
        case MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT:
            batteryLevelUpdatePercent((MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T *) message);
            break;
        default:
            break;
    }
}

void batteryStatusNotifyInit(void) {
    /* Set up task handler */
    batteryStatusTaskData.task.handler = batteryStatusNotifyHandleMessage;

    batteryStatusTaskData.Client.task = &batteryStatusTaskData.task;
    batteryStatusTaskData.Client.hysteresis = 1;
    batteryStatusTaskData.Client.representation = battery_level_repres_percent;

    appBatteryRegister(&batteryStatusTaskData.Client);
}

void batteryStatusNotifyEnableReport(void) {
    DEBUG_PRINTF("AT Send:[%s]\n", batt_enable_string);
    HfpAtCmdRequest(hfp_primary_link, batt_enable_string);
}











