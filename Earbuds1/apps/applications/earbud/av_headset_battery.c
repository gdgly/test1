/*!
\copyright  Copyright (c) 2008 - 2018 Qualcomm Technologies International, Ltd.\n
            All Rights Reserved.\n
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       av_headset_battery.c
\brief      Battery monitoring
*/

#include <adc.h>

#include "av_headset.h"
#include "av_headset_battery.h"
#include "av_headset_log.h"
#include "av_headset_test.h"
#include "hydra_macros.h"
#include "panic.h"

/*! Whilst the filter is being filled read at this rate */
#define BATTERY_READ_PERIOD_INITIAL (0)

/*! Enumerated type for messages sent within the headset battery 
    handler only. */
enum headset_battery_internal_messages
{
        /*! Message sent to trigger an intermittent battery measurement */
    MESSAGE_BATTERY_INTERNAL_MEASUREMENT_TRIGGER = 1,

    MESSAGE_BATTERY_TEST_PROCESS_READING = MESSAGE_BATTERY_PROCESS_READING,
};

/*! TRUE when the filter is filled with results */
#define FILTER_IS_FULL(battery) ((battery)->filter.index >= BATTERY_FILTER_LEN)

/*! Add a client to the list of clients */
static bool appBatteryClientAdd(batteryTaskData *battery, batteryRegistrationForm *form)
{
    batteryRegisteredClient *new = calloc(1, sizeof(*new));
    if (new)
    {
        new->form = *form;
        new->next = battery->client_list;
        battery->client_list = new;
        return TRUE;
    }
    return FALSE;
}

/*! Remove a client from the list of clients */
static void appBatteryClientRemove(batteryTaskData *battery, Task task)
{
    batteryRegisteredClient **head;
    for (head = &battery->client_list; *head != NULL; head = &(*head)->next)
    {
        if ((*head)->form.task == task)
        {
            batteryRegisteredClient *toremove = *head;
            *head = (*head)->next;
            free(toremove);
            break;
        }
    }
}

#ifdef CONFIG_STAROT
/*
#define VOL_CHARGE_LEN 70
#define SEC_CHARGE_LEN 31
#define VOL_DISCHARGE 100
const uint16 vol_charge[VOL_CHARGE_LEN] = {
        3454 ,3684 ,3772 ,3881 ,3916 ,3941 ,3956 ,3964 ,3968 ,3977 ,3982 ,3989 ,3995 ,4001 ,4008 ,4014 ,
        4027 ,4033 ,4039 ,4045 ,4051 ,4055 ,4065 ,4070 ,4074 ,4078 ,4082 ,4090 ,4094 ,4098 ,4102 ,4106 ,
        4110 ,4118 ,4122 ,4127 ,4132 ,4137 ,4141 ,4152 ,4157 ,4162 ,4168 ,4174 ,4179 ,4191 ,4197 ,4202 ,
        4209 ,4215 ,4222 ,4235 ,4241 ,4248 ,4255 ,4263 ,4270 ,4285 ,4293 ,4301 ,4309 ,4317 ,4325 ,4342 ,
        4350 ,4359 ,4368 ,4377 ,4380 ,4390
};
const uint8 sec_charge[SEC_CHARGE_LEN] = {2,4,6,10,12,16,18,22,24,28,32,38,42,46,
                              50,56,62,68,74,82,90,98,108,118,130,144,162,182,208,
                              242};
static uint16 vol_discharge[VOL_DISCHARGE] = {
        4300 ,4270 ,4255 ,4242 ,4229 ,4215 ,4203 ,4192 ,4179 ,4168 ,4157 ,4146 ,4134 ,4123 ,4112 ,4100 ,
        4090 ,4080 ,4067 ,4057 ,4047 ,4036 ,4025 ,4015 ,4005 ,3994 ,3983 ,3973 ,3961 ,3948 ,3937 ,3926 ,
        3915 ,3906 ,3898 ,3889 ,3881 ,3874 ,3867 ,3859 ,3852 ,3845 ,3837 ,3831 ,3825 ,3818 ,3812 ,3806 ,
        3800 ,3793 ,3788 ,3783 ,3778 ,3772 ,3767 ,3762 ,3757 ,3752 ,3748 ,3743 ,3738 ,3734 ,3730 ,3726 ,
        3722 ,3718 ,3714 ,3710 ,3706 ,3703 ,3699 ,3694 ,3690 ,3686 ,3680 ,3675 ,3670 ,3665 ,3658 ,3652 ,
        3645 ,3638 ,3631 ,3624 ,3617 ,3608 ,3599 ,3589 ,3579 ,3564 ,3549 ,3532 ,3508 ,3484 ,3453 ,3414 ,
        3355 ,3286 ,3192 ,3019
};*/
#define VOL_CHARGE_LEN 35
#define SEC_CHARGE_LEN 15
#define VOL_DISCHARGE 50
const uint16 vol_charge[VOL_CHARGE_LEN] = {
        3454 ,3772 ,3916 ,3956 ,3968 ,3982 ,3995 ,4008 ,
        4027 ,4039 ,4051 ,4065 ,4074 ,4082 ,4094 ,4102 ,
        4110 ,4122 ,4132 ,4141 ,4157 ,4168 ,4179 ,4197 ,
        4209 ,4222 ,4241 ,4255 ,4270 ,4293 ,4309 ,4325 ,
        4350 ,4368 ,4380
};
const uint8 sec_charge[SEC_CHARGE_LEN] = {2,6,12,18,24,32,42,
                              50,62,74,90,108,130,162,208};
static uint16 vol_discharge[VOL_DISCHARGE] = {
        4270 ,4242 ,4215 ,4192 ,4168 ,4146 ,4123 ,4100 ,
        4080 ,4057 ,4036 ,4015 ,3994 ,3973 ,3948 ,3926 ,
        3906 ,3889 ,3874 ,3859 ,3845 ,3831 ,3818 ,3806 ,
        3793 ,3783 ,3772 ,3762 ,3752 ,3743 ,3734 ,3726 ,
        3718 ,3710 ,3703 ,3694 ,3686 ,3675 ,3665 ,3652 ,
        3638 ,3624 ,3608 ,3589 ,3564 ,3532 ,3484 ,3414 ,
        3286 ,3019
};
static int8  g_last_percent = -1;
static uint32 g_charge_calc_ticks = 0;
#include "param.h"
extern UserPrmPtr GetUserParam(void);
static uint8 toPercentage(uint16 voltage)
{
    int i;
    uint8 iPercent;
    uint32 diff;
    chargerTaskData *theCharger = appGetCharger();

    UserPrmPtr prm = GetUserParam();

    if(theCharger->is_charging)
    {
        //充电
        for(i = 0; i < VOL_CHARGE_LEN; i++) {
            if(voltage <= vol_charge[i])
                break;
        }

        if(i >= VOL_CHARGE_LEN)  {
            if(prm->electricQuantity >= 70)
               g_last_percent = prm->electricQuantity;

            if(g_last_percent < VOL_CHARGE_LEN)
                g_last_percent = VOL_CHARGE_LEN;

            if(g_charge_calc_ticks == 0) {
                g_charge_calc_ticks = VmGetClock();
                if(g_last_percent > VOL_CHARGE_LEN) {
                    diff = g_last_percent - VOL_CHARGE_LEN -1;
                    diff = sec_charge[diff]*10000;
                    g_charge_calc_ticks = VmGetClock() - diff;
                }
            }

            diff = (VmGetClock()-g_charge_calc_ticks) / 10000;      // 单位换算为10SEC
            for(i = 0; i < SEC_CHARGE_LEN; i++) {
              if(diff < sec_charge[i])
                break;
            }

            if(g_last_percent == -1){//防止充电中重启，当充电没满继续充电直到充电完成，当充电没满被拿出会出现突然掉电现象。
                iPercent = 100;
                g_last_percent = 100;
            }else{
                iPercent = VOL_CHARGE_LEN*2 + i*2;
            }

        }else{
            iPercent = i*2;
            g_charge_calc_ticks = 0;
        }
        if(iPercent < g_last_percent && g_last_percent != -1)
            iPercent = g_last_percent;

    }
    else {
        //放电
        if(g_charge_calc_ticks)
            g_charge_calc_ticks = 0;
        for(i = 0; i < VOL_DISCHARGE; i++) {
                if(voltage > vol_discharge[i])
                        break;
        }
        iPercent = 100 - i*2;
        if(iPercent > g_last_percent && g_last_percent != -1)
            iPercent = g_last_percent;
   }
    g_last_percent = iPercent;

    if(g_last_percent>=70)
        prm->electricQuantity = g_last_percent;
    else
        prm->electricQuantity = 0;

    return g_last_percent;
}
#else
static uint8 toPercentage(uint16 voltage)
{
    uint16 critical = appConfigBatteryVoltageCritical();
    uint16 charged = appConfigBatteryFullyCharged();

    if (voltage < critical)
        voltage = critical;
    else if (voltage > charged)
        voltage = charged;

    return (100UL * (uint32)(voltage - critical)) / (uint32)(charged - critical);
}
#endif

static battery_level_state toState(uint16 voltage)
{
    if (FILTER_IS_FULL(appGetBattery()))
    {
        if (voltage < appConfigBatteryVoltageCritical())
            return battery_level_too_low;
        if (voltage < appConfigBatteryVoltageLow())
            return battery_level_critical;
        if (voltage < appConfigBatteryVoltageOk())
            return battery_level_low;

        return battery_level_ok;
    }

    return battery_level_unknown;
}

/* TRUE if the current value is less than the threshold taking into account hysteresis */
static bool ltThreshold(uint16 current, uint16 threshold, uint16 hysteresis)
{
    return current < (threshold - hysteresis);
}

/* TRUE if the current value is greater than the threshold taking into account hysteresis. */
static bool gtThreshold(uint16 current, uint16 threshold, uint16 hysteresis)
{
    return current > (threshold + hysteresis);
}

/* TRUE if the current value is outside the threshold taking into account hysteresis */
static bool thresholdExceeded(uint16 current, uint16 threshold, uint16 hysteresis)
{
    return ltThreshold(current, threshold, hysteresis) ||
           gtThreshold(current, threshold, hysteresis);
}

/* Determine if the transition should be made between last_state and
   new_state. For the transition to happen, the current battery voltage must
   exceed the state defined voltage level by the amount of hysteresis. 

   If the state has jumped two levels, then we assume the level has jumped and
   this is acceptable.
 */
static bool stateUpdateIsRequired(battery_level_state last_state,
                                  battery_level_state new_state,
                                  uint16 voltage,
                                  uint16 hysteresis)
{
    if (   new_state == battery_level_unknown
        || new_state == last_state)
    {
        return FALSE;
    }
    
    if (last_state == battery_level_unknown)
    {
        return TRUE;
    }

    switch (last_state)
    {
        case battery_level_too_low:
            switch (new_state)
            {
                case battery_level_critical:
                    return gtThreshold(voltage, appConfigBatteryVoltageCritical(), hysteresis);
                case battery_level_low:
                case battery_level_ok:
                    return TRUE;
                default:
                    break;
            }
            break;

        case battery_level_critical:
            switch (new_state)
            {
                case battery_level_too_low:
                    return ltThreshold(voltage, appConfigBatteryVoltageCritical(), hysteresis);
                case battery_level_low:
                    return gtThreshold(voltage, appConfigBatteryVoltageLow(), hysteresis);
                case battery_level_ok:
                    return TRUE;
                default:
                    break;
            }
            break;

        case battery_level_low:
            switch (new_state)
            {
                case battery_level_too_low:
                    return TRUE;
                case battery_level_critical:
                    return ltThreshold(voltage, appConfigBatteryVoltageLow(), hysteresis);
                case battery_level_ok:
                    return gtThreshold(voltage, appConfigBatteryVoltageOk(), hysteresis);
                default:
                    break;
            }
            break;

        case battery_level_ok:
            switch (new_state)
            {
                case battery_level_too_low:
                case battery_level_critical:
                    return TRUE;
                case battery_level_low:
                    return ltThreshold(voltage, appConfigBatteryVoltageOk(), hysteresis);
                default:
                    break;
            }
            break;

        default:
            break;
    }
    return FALSE;
}

/*! Iterate through the list of clients, sending battery level messages when
    the representation criteria is met */
static void appBatteryServiceClients(batteryTaskData *battery)
{
    batteryRegisteredClient *client = NULL;
    uint16 voltage = appBatteryGetVoltage();
    for (client = battery->client_list; client != NULL; client = client->next)
    {
        uint16 hysteresis = client->form.hysteresis;
        switch (client->form.representation)
        {
            case battery_level_repres_voltage:
                if (thresholdExceeded(voltage, client->last.voltage, hysteresis))
                {
                    MESSAGE_MAKE(msg, MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE_T);
                    msg->voltage_mv = voltage;
                    client->last.voltage = voltage;
                    MessageSend(client->form.task, MESSAGE_BATTERY_LEVEL_UPDATE_VOLTAGE, msg);
                }
            break;
            case battery_level_repres_percent:
            {
                uint8 percent = toPercentage(voltage);
                if (thresholdExceeded(percent, client->last.percent, hysteresis))
                {
                    MESSAGE_MAKE(msg, MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT_T);
                    msg->percent = percent;
                    client->last.percent = percent;
                    MessageSend(client->form.task, MESSAGE_BATTERY_LEVEL_UPDATE_PERCENT, msg);
                }
            }
            break;
            case battery_level_repres_state:
            {
                battery_level_state new_state = toState(voltage);
                if (stateUpdateIsRequired(client->last.state, new_state, voltage, hysteresis))
                {
                    MESSAGE_MAKE(msg, MESSAGE_BATTERY_LEVEL_UPDATE_STATE_T);
                    msg->state = new_state;
                    client->last.state = new_state;
                    MessageSend(client->form.task, MESSAGE_BATTERY_LEVEL_UPDATE_STATE, msg);
                }
            }
            break;
        }
    }
}

static void appBatteryScheduleNextMeasurement(batteryTaskData *battery)
{
    uint32 delay = FILTER_IS_FULL(battery) ?
                        battery->period :
                        BATTERY_READ_PERIOD_INITIAL;
    MessageSendLater(&battery->task, MESSAGE_BATTERY_INTERNAL_MEASUREMENT_TRIGGER,
                        NULL, delay);
}

/*! Return TRUE if a new voltage is available,with enough samples that the result
    should be stable. This waits for the filter to be full. */
static bool appBatteryAdcResultHandler(batteryTaskData *battery, MessageAdcResult* result)
{
    uint16 reading = result->reading;

    switch (result->adc_source)
    {
        case adcsel_pmu_vbat_sns:
        {
            uint32 index = battery->filter.index & BATTERY_FILTER_MASK;
            uint16 vbatt_mv = (uint16)((uint32)VmReadVrefConstant() * reading / battery->vref_raw);

            battery->filter.accumulator -= battery->filter.buf[index];
            battery->filter.buf[index] = vbatt_mv;
            battery->filter.accumulator += vbatt_mv;
            battery->filter.index++;
            /* See the logic in appBatteryGetVoltage():
               0<=index<BATTERY_FILTER_LEN is only used when filling the filter.
               When incrementing to BATTERY_FILTER_LEN, initialisation is complete.
               When wrapping to 0, jump over the initialisation index range. */
            if (battery->filter.index == BATTERY_FILTER_LEN)
            {
                MessageSend(appGetAppTask(), MESSAGE_BATTERY_INIT_CFM, NULL);
            }
            if (battery->filter.index == 0)
            {
                battery->filter.index = BATTERY_FILTER_LEN;
            }
            appBatteryScheduleNextMeasurement(battery);
            return FILTER_IS_FULL(battery);
        }

        case adcsel_vref_hq_buff:
            battery->vref_raw = reading;
            break;

        default:
            DEBUG_LOGF("appBatteryAdcResultHandler unexpected source - %d",result->adc_source);
            break;
    }
    return FALSE;
}

static void appBatteryHandleMessage(Task task, MessageId id, Message message)
{
    batteryTaskData *battery = (batteryTaskData *)task;
    if (battery->period != 0)
    {
        switch (id)
        {
            case MESSAGE_ADC_RESULT:
                if (appBatteryAdcResultHandler(battery, (MessageAdcResult*)message))
                {
                    appBatteryServiceClients(battery);
                }
                break;

            case MESSAGE_BATTERY_TEST_PROCESS_READING:
                appBatteryServiceClients(battery);
                break;

            case MESSAGE_BATTERY_INTERNAL_MEASUREMENT_TRIGGER:
                /* Start immediate battery reading, note vref is read first */
                AdcReadRequest(&battery->task, adcsel_vref_hq_buff, 0, 0);
                AdcReadRequest(&battery->task, adcsel_pmu_vbat_sns, 0, 0);
                break;

            default:
                /* An unexpected message has arrived - must handle it */
                appHandleUnexpected(id);
                break;
        }
    }
}

void appBatteryInit(void)
{
    DEBUG_LOG("appBatteryMonInit");
    batteryTaskData *battery = appGetBattery();
    memset(battery, 0, sizeof(*battery));

    /* Set up task handler */
    battery->task.handler = appBatteryHandleMessage;
    battery->period = appConfigBatteryReadPeriodMs();

    appBatteryScheduleNextMeasurement(battery);
}

void appBatterySetPeriod(uint16 period)
{
    batteryTaskData *battery = appGetBattery();
    DEBUG_LOGF("appBatteryMonPeriod %d", period);
    if (period == 0)
    {
        /* Reset the filter data */
        memset(&battery->filter, 0, sizeof(battery->filter));
    }
    else if (battery->period == 0)
    {
        /* Restart */
        appBatteryScheduleNextMeasurement(battery);
    }
    battery->period = period;
}

uint16 appBatteryGetVoltage(void)
{
    if (appTestBatteryVoltage)
    {
        return appTestBatteryVoltage;
    }
    else
    {
        batteryTaskData *battery = appGetBattery();
        uint16 index = battery->filter.index;
        uint32 accumulator = battery->filter.accumulator;

        if (FILTER_IS_FULL(battery))
        {
            return accumulator / BATTERY_FILTER_LEN;
        }
        return index == 0 ? 0 : accumulator / index;
    }
}

battery_level_state appBatteryGetState(void)
{
    uint16 voltage = appBatteryGetVoltage();

    return toState(voltage);
}


uint8 appBatteryConvertLevelToPercentage(uint16 level_mv)
{
    return toPercentage(level_mv);
}


uint8 appBatteryGetPercent(void)
{
    uint16 voltage_mv = appBatteryGetVoltage();

    return toPercentage(voltage_mv);
}

bool appBatteryRegister(batteryRegistrationForm *client)
{
    batteryTaskData *battery = appGetBattery();
    if (appBatteryClientAdd(battery, client))
    {
        appBatteryServiceClients(battery);
        return TRUE;
    }
    return FALSE;
}

extern void appBatteryUnregister(Task task)
{
    batteryTaskData *battery = appGetBattery();
    appBatteryClientRemove(battery, task);
}
