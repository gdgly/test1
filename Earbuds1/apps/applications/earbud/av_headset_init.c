/*!
\copyright  Copyright (c) 2008 - 2017 Qualcomm Technologies International, Ltd.
            All Rights Reserved.
            Qualcomm Technologies International, Ltd. Confidential and Proprietary.
\version    
\file       av_headset_init.c
\brief      Initialisation module
*/

#ifndef AV_HEADSET_INIT_C
#define AV_HEADSET_INIT_C

#include <panic.h>
#include <pio.h>
#include <stdio.h>
#include <feature.h>

#include "av_headset.h"
#include "av_headset_log.h"
#include "gaia.h"
#include "apollo.h"

/*! \brief Initialsiation function entry
*/
typedef struct
{
    void (*init)(void);                 /*!< Initialisation function to call */
    uint16 message_id;                  /*!< Message ID to wait for, 0 if no message required */
    void (*handler)(Message message);   /*!< Function to call when message with above ID is received */
} appInitTableEntry;

static void appPioInit(void)
{
#ifndef USE_BDADDR_FOR_LEFT_RIGHT
    /* Make PIO2 an input with pull-up */
    PioSetMapPins32Bank(0, 1UL << appConfigHandednessPio(), 1UL << appConfigHandednessPio());
    PioSetDir32Bank(0, 1UL << appConfigHandednessPio(), 0);
    PioSet32Bank(0, 1UL << appConfigHandednessPio(), 1UL << appConfigHandednessPio());
    DEBUG_LOGF("appPioInit, left %d, right %d", appConfigIsLeft(), appConfigIsRight());
#endif
}

static void appLicenseCheck(void)
{
    if (FeatureVerifyLicense(APTX_CLASSIC))
        DEBUG_LOG("appLicenseCheck: aptX Classic is licensed, aptX A2DP CODEC is enabled");
    else
        DEBUG_LOG("appLicenseCheck: aptX Classic not licensed, aptX A2DP CODEC is disabled");

    if (FeatureVerifyLicense(APTX_CLASSIC_MONO))
        DEBUG_LOG("appLicenseCheck: aptX Classic Mono is licensed, aptX TWS+ A2DP CODEC is enabled");
    else
        DEBUG_LOG("appLicenseCheck: aptX Classic Mono not licensed, aptX TWS+ A2DP CODEC is disabled");

    if (FeatureVerifyLicense(CVC_RECV))
        DEBUG_LOG("appLicenseCheck: cVc Receive is licensed");
    else
        DEBUG_LOG("appLicenseCheck: cVc Receive not licensed");

    if (FeatureVerifyLicense(CVC_SEND_HS_1MIC))
        DEBUG_LOG("appLicenseCheck: cVc Send 1-MIC is licensed");
    else
        DEBUG_LOG("appLicenseCheck: cVc Send 1-MIC not licensed");

    if (FeatureVerifyLicense(CVC_SEND_HS_2MIC_MO))
        DEBUG_LOG("appLicenseCheck: cVc Send 2-MIC is licensed");
    else
        DEBUG_LOG("appLicenseCheck: cVc Send 2-MIC not licensed");
}

/*! \brief Connection library initialisation */
static void appConnectionInit(void)
{
    static const msg_filter filter = {msg_group_acl | msg_group_mode_change};

    /* Initialise the Connection Manager */
#if defined(APP_SECURE_CONNECTIONS)
    ConnectionInitEx3(appGetAppTask(), &filter, appConfigMaxPairedDevices(), CONNLIB_OPTIONS_SC_ENABLE);
#else
    ConnectionInitEx3(appGetAppTask(), &filter, appConfigMaxPairedDevices(), CONNLIB_OPTIONS_NONE);
#endif
}

#ifdef LOCAL_NAME_CONFIGURATION_UNSUPPORTED
/*************************************************************************
NAME    
    writeLocalName
    
DESCRIPTION
    Write a default local device name when there is no other configuration mechanism supported.
*/
static void writeLocalName(void)
{
    char new_name[] = "@@@QTIL_earbudApp@@@";

    ConnectionChangeLocalName(strlen(new_name), (uint8 *)new_name);
}
#endif

/*! \brief Handle Connection library confirmation message */
static void appInitHandleClInitCfm(Message message)
{
    CL_INIT_CFM_T *cfm = (CL_INIT_CFM_T *)message;
    if (cfm->status != success)
        Panic();

    /* Set the class of device to indicate this is a headset */
    ConnectionWriteClassOfDevice(AUDIO_MAJOR_SERV_CLASS | RENDER_MAJOR_SERV_CLASS |
                                 AV_MAJOR_DEVICE_CLASS | HEADSET_MINOR_DEVICE_CLASS);

    /* Allow SDP without security, requires authorisation */
    ConnectionSmSetSecurityLevel(0, 1, ssp_secl4_l0, TRUE, TRUE, FALSE);

    /* Reset security mode config - always turn off debug keys on power on */
    ConnectionSmSecModeConfig(appGetAppTask(), cl_sm_wae_acl_owner_none, FALSE, TRUE);
    
#ifdef LOCAL_NAME_CONFIGURATION_UNSUPPORTED
    writeLocalName();
#endif
}


/*! \brief Handle GATT library confirmation message */
static void appInitHandleGattInitCfm(Message message)
{
    UNUSED(message);

    appGattSetAdvertisingMode(APP_ADVERT_RATE_SLOW);
}

#ifdef USE_BDADDR_FOR_LEFT_RIGHT

#ifdef CONFIG_I2CADDR_FOR_LEFT_RIGHT
static void appConfigInit(void)
{
    int ret = max20340_get_left_or_right();
    if(1 == ret) {
        appGetInit()->appInitIsLeft = 1;
        DEBUG_LOG("is left ear");
    } else if(2 == ret) {
        appGetInit()->appInitIsLeft = 0;
        DEBUG_LOG("is right ear");
    } else
        DEBUG_LOG("ERROR Get MAX20340 for LEFT/RIGHT");

    MessageSend(appGetAppTask(), CL_DM_LOCAL_BD_ADDR_CFM, NULL);
}

static void appInitHandleClDmLocalBdAddrCfm(Message message)
{
    (void)message;
}

#else
static void appConfigInit(void)
{
    /* Get local device address */
    ConnectionReadLocalAddr(appGetAppTask());
}

static void appInitHandleClDmLocalBdAddrCfm(Message message)
{
    CL_DM_LOCAL_BD_ADDR_CFM_T *cfm = (CL_DM_LOCAL_BD_ADDR_CFM_T *)message;
    if (cfm->status != success)
        Panic();

    appGetInit()->appInitIsLeft = cfm->bd_addr.lap & 0x01;

    DEBUG_LOGF("appInit, bdaddr %04x:%02x:%06x",
                    cfm->bd_addr.nap, cfm->bd_addr.uap, cfm->bd_addr.lap);
    DEBUG_LOGF("appInit, left %d, right %d", appConfigIsLeft(), appConfigIsRight());
}
#endif
#endif

#ifdef INIT_DEBUG
/*! Debug function blocks execution until appInitDebugWait is cleared:
    apps1.fw.env.vars['appInitDebugWait'].set_value(0) */
static void appInitDebug(void)
{
    volatile static bool appInitDebugWait = TRUE;
    while(appInitDebugWait);
}
#endif

#ifdef AUDIO_PARAM_TUNE
extern void qact_tune_for_mic(void);
#endif

#ifdef HFP_BATTERY_STATUS_NOTIFY
extern void batteryStatusNotifyInit(void);
#endif

#ifdef CONFIG_STAROT
extern void subPhyTaskInit(void);
#endif

/*! \brief Table of initialisation functions */
static const appInitTableEntry appInitTable[] =
{
#ifdef INIT_DEBUG
    {appInitDebug,          0, NULL},
#endif
    {appPioInit,            0, NULL},
    {appUiInit,             0, NULL},
    {appLicenseCheck,       0, NULL},
#ifdef CONFIG_STAROT
    {subPhyTaskInit, 0, NULL},
#endif
#ifdef INCLUDE_TEMPERATURE
    {appTemperatureInit,    TEMPERATURE_INIT_CFM, NULL},
#endif
    {appBatteryInit,        MESSAGE_BATTERY_INIT_CFM, NULL},
#ifdef CONFIG_HW_UART
    {uartInit,              0, NULL},                     // 一 串口调试输出放前面
#endif
#ifdef INCLUDE_CHARGER
    {appChargerInit,        0, NULL},
#endif
    {appLedInit,            0, NULL},

#ifdef ENABLE_APOLLO
    {apollo_int_io_init,    APOLLO_INIT_CFM, NULL},       // 二
#endif
#ifdef CONFIG_STAROT
   // {appSubUIInit,          0, NULL},                     // 三 必须在ParamConfigInit之前
#endif
#ifdef HAVE_MAX20340
    {max20340_init,             0, NULL},
#endif
#ifdef CONFIG_LIS25BA
    {lis25Init,             0, NULL},
    {lis25Test,             0, NULL},
#endif
#if defined(HAVE_BMA400) || defined(HAVE_LIS2DW12)
    {tap_func_init,             0, NULL},
#endif
#ifdef CONFIG_SHELL_UART
    {ShellCmdInit,          0, NULL},
#endif
    {appPowerInit,          APP_POWER_INIT_CFM, NULL},
    {appPhyStateInit,       PHY_STATE_INIT_CFM, NULL},
    {appConnectionInit,     CL_INIT_CFM, appInitHandleClInitCfm},
#ifdef USE_BDADDR_FOR_LEFT_RIGHT
    {appConfigInit,         CL_DM_LOCAL_BD_ADDR_CFM, appInitHandleClDmLocalBdAddrCfm},
#endif
#ifdef CONFIG_STAROT
    {appSubUIInit,          0, NULL},                     // 三 必须在ParamConfigInit之前
    {ParamConfigInit,       CL_DM_LOCAL_BD_ADDR_CFM, ParamInitHandleClDmLocalBdAddrCfm},
#endif
#ifdef HAVE_EM20168//放到subui后面，因为要读flash
    {EM20168_init,             0, NULL},
#endif
#ifdef HAVE_UCS146E0//放到subui后面，因为要读flash
    {Ucs146e0_init,             0, NULL},
#endif
    {appLinkPolicyInit,     0, NULL},
    {appConManagerInit,     0, NULL},
    {appConnRulesInit,      0, NULL},
    {appDeviceInit,         0, NULL},
    {appScanManagerInit,    0, NULL},
    {appAdvManagerInit,     APP_ADVMGR_INIT_CFM, NULL},
    {appAvInit,             AV_INIT_CFM, NULL},
    {appPeerSigInit,        0, NULL},
    {appPairingInit,        PAIRING_INIT_CFM, NULL},
    {appPeerSyncInit,       0, NULL},
    {appHfpInit,            APP_HFP_INIT_CFM, NULL},
    {appHandsetSigInit,     0, NULL},
    {appKymeraInit,         0, NULL},
    {appSmInit,             0, NULL},
    {appScoFwdInit,         SFWD_INIT_CFM, NULL},
    {TransportMgrInit,      0, NULL},
#ifdef INCLUDE_DFU
    {appGaiaInit,           APP_GAIA_INIT_CFM, NULL},   // Gatt needs GAIA
#endif
#ifdef INCLUDE_GATT
    {appGattInit,           APP_GATT_INIT_CFM, appInitHandleGattInitCfm},
#endif
#ifdef INCLUDE_DFU
    {appUpgradeInit,        UPGRADE_INIT_CFM, NULL},    // Upgrade wants to start a connection (can be gatt)
#endif
#ifdef CUMMPC_PC_USB
    {CummuInit,             0, NULL},
#endif
#ifdef AUDIO_PARAM_TUNE
    {qact_tune_for_mic, 0, NULL},
#endif
#ifdef HFP_BATTERY_STATUS_NOTIFY
    {batteryStatusNotifyInit, 0, NULL},
#endif
    {NULL,                  0, NULL}
};

/*! \brief Action next entry in init table */
static void appInitNextEntry(void)
{
    initData *theInit = appGetInit();

    /* Move to next entry */
    theInit->init_index += 1;

    while (appInitTable[theInit->init_index].init)
    {
        /* Call init function */
        appInitTable[theInit->init_index].init();
        theInit->id = appInitTable[theInit->init_index].message_id;
        if (theInit->id)
            return;

        /* Move to next entry */
        theInit->init_index += 1;
    }

    MessageSend(appGetAppTask(), INIT_CFM, NULL);

    theInit->initialised = APP_INIT_COMPLETED_MAGIC;
}

void appInit(void)
{    
    initData *theInit = appGetInit();
    theInit->init_index = 0xFF;
    theInit->id = 0;

    appInitNextEntry();
}

uint16 appInitWaitingForMessageId(void)
{
    /* Return ID of message we're waiting for */
    return appGetInit()->id;
}

void appInitHandleMessage(MessageId id, Message message)
{
    initData *theInit = appGetInit();
    PanicFalse(id == theInit->id);

    /* Call message handler function */
    if (appInitTable[theInit->init_index].handler != NULL)
        appInitTable[theInit->init_index].handler(message);

    /* Advance to next entry in table */
    appInitNextEntry();
}

#endif // AV_HEADSET_INIT_C
