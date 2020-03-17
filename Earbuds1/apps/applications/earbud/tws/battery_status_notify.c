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

enum {
    INIT_NOTIFY_DEVICE_BATTERY = 0X100,
};

// endregion

static void sendBatteryByAtCmd(uint8 percent) {
    UNUSED(percent);
    char *data = (char*)PanicUnlessMalloc(sizeof(batt_level_string));
    if (NULL != data) {
        memcpy(data, batt_level_string, sizeof(batt_level_string));
        DEBUG_LOGF("AT: Send Cmd battery percent %d\n", percent);
        data[BATTERY_POS] = ((percent - 1) / 10) + 0x30;
        DEBUG_PRINTF("AT Send:[%s]\n", data);
        HfpAtCmdRequest(hfp_primary_link, data);
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

        case INIT_NOTIFY_DEVICE_BATTERY:
            sendBatteryByAtCmd(appBatteryGetPercent());
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
    sendBatteryByAtCmd(appBatteryGetPercent());
}


// region 扩展

/*
HFP命令AT+IPHONEACCEV
        描述：报告耳机的状态变更
        发起者：耳机
        格式：AT+IPHONEACCEV=[Number of key/value pairs ],[key1 ],[val1 ],[key2 ],[val2 ],...
参数：
Number of key/value pairs : 接下来参数的数量
        key: 被报告状态变化的类型
1 = 电量等级
2 = 暂停状态
        val: 更改的值
        Battery events:0-9之间数字的字符串 A string value between '0' and '9'.
Dock state: 0 = undocked, 1 = docked.
Example: AT+IPHONEACCEV=1,1,3
*/

// endregion
