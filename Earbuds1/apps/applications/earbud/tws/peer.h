//
// Created by Administrator on 2019/11/11.
//

#ifndef EARBUDS1_PEER_H
#define EARBUDS1_PEER_H

#include "av_headset_av.h"

////////////////////////////////////////////////////////////////////////////////
/////////////// 左右耳机数据命令
////////////////////////////////////////////////////////////////////////////////
typedef struct {
    Task    client_task;           /*!< Task to receive any response */
    uint8   command;
    uint8   length;                // 总数据长度，包括command+length+data
    uint8   data[2];
} PEER_SIG_INTERNAL_TXDATA_REQ_T;

#define AVRCP_PEER_CMD_TXDATA                   0x69
#define AVRCP_PEER_CMD_TXDATA_BASE_SIZE         2  // command + length

enum {PEERTX_CMD_SYNCGAIA=0,         /* 主副耳机发送gaia状态   [payload[0], 0:disconnect 1:connect */
      PEERTX_CMD_WAKEUP_SYS,         /* 副耳机发送系统唤醒命令 [playload[0]:  0:keywakeup, 1:voice wakeup */
      PEERTX_CMD_WAKEUP_APP,         /* 副耳机发送唤醒APP命令  [playload[0]:  0:keywakeup, 1:voice wakeup */
      PEERTX_CMD_SYNC_BLEPAIR,       /* 同步BLE配对码 */
      PEERTX_CMD_SYNC_DOUBLE_CLICK,  /* 同步双击 */
      PEERTX_CMD_SYNC_CASEST,        /* 一只耳机在盒中，另外一只不在，可以做同步 */
      PEERTX_CMD_SENSOR_ENB,         /* 佩戴使能同步到对方耳机 */
};
void appPeerSigTxDataCommand(Task task, const bdaddr *peer_addr, uint8 command, uint16 size_payload, const uint8 *payload);
void appPeerSigTxDataCommandExt(Task task,uint8 command, uint16 size_payload, const uint8 *payload);
void appPeerSigTxDataCommandUi(uint8 command, uint8 payload);  // task为UI 仅一个字节payhload
#define appPeerSigTxWakeupSys(apo)   appPeerSigTxDataCommandUi(PEERTX_CMD_WAKEUP_SYS, (apo))
#define appPeerSigTxWakeupApp(apo)   appPeerSigTxDataCommandUi(PEERTX_CMD_WAKEUP_APP, (apo))
/*
 * 同步耳机双击的配置
 */
void appPeerSigTxSyncDoubleClick(Task task, uint8 left, uint8 right);

void appPeerSigTxSyncPair(Task task);          // 同步配对信息
void appPeerSigTxDataRequest(PEER_SIG_INTERNAL_TXDATA_REQ_T *req);           // 发送方： 请求发送给对方
bool appPeerSigRxDataCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);        // 接收方： 接收数据处理
void appPeerSigTxDataConfirm(Task task, peerSigStatus status);               // 发送方： 获取对方返回值
bool appUiRecvPeerCommand(PEER_SIG_INTERNAL_TXDATA_REQ_T *req);              // 接收方： 返回给上层处理

#ifdef CONFIG_SINGLE_SYNC_BLE_PAIR   // 保存多组配对码后，直接一次性同步
////////////////////////////////////ble配对信息////////////////////////////////////////////

/******************************************************************************
 * Peer Signalling Message Definitions
 ******************************************************************************/
#define AVRCP_PEER_CMD_BLE_CONFIG                 0x70
#define AVRCP_PEER_CMD_BLE_CONFIG_SIZE            8    /// advCode + bondCode

typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    int advCode;
    int bondCode;
} PEER_SIG_INTERNAL_BLE_CONFIG_REQ_T;

void appPeerSigTxBleConfigRequest(Task task, const bdaddr *peer_addr, int advCode, int bondCode);
void appPeerSigHandleInternalBleConfigRequest(PEER_SIG_INTERNAL_BLE_CONFIG_REQ_T *req);
bool appPeerSigHandleBleConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
void appPeerSigMsgBleConfigConfirmation(Task task, peerSigStatus status);
#endif
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////通用配置项////////////////////////////////////////////

#define AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE                  12

typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    uint8 apollo_config;
    uint8 wear_config;
    uint8 assistant_type;
    uint8 dontused;
    uint32 apollo_timestamp;
    uint32 wear_timestamp;
} PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T;

#define AVRCP_PEER_CMD_NORMAL_CONFIG                       0x72

void appPeerSigTxNormalConfigRequest(Task task, const bdaddr *peer_addr);
void appPeerSigHandleInternalNormalConfigRequest(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T *req);
bool appPeerSigHandleNormalConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
void appPeerSigMsgNormalConfigConfirmation(Task task, peerSigStatus status);

/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////双击消息项////////////////////////////////////////////
#define AVRCP_PEER_DOUBLE_CLICK_WAKEUP                 0x73
#define AVRCP_PEER_DOUBLE_CLICK_WAKEUP_SYSTEM          0x74
//#define AVRCP_PEER_DOUBLE_CLICK_WAKEUP_SIZE            10
typedef struct
{
    Task client_task;           /*!< Task to receive any response */
} PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T;

void appPeerSigTxDoubleClickWakeupRequest(Task task, const bdaddr *peer_addr, int num);
void appPeerSigHandleInternalDoubleClickWakeupRequest(PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T *req);
void appPeerSigHandleInternalDoubleClickWakeupSystemRequest(PEER_SIG_DOUBLE_CLICK_WAKEUP_REQ_T *req);
bool appPeerSigHandleDoubleClickWakeupCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
bool appPeerSigHandleDoubleClickWakeupSystemCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
void appPeerSigMsgDoubleClickWakeupConfirmation(Task task, peerSigStatus status);

/////////////////////////////////////////////////////////////////////////////////////////

// region 统一发送请求数据

typedef struct AVRCP_PEER_CMD_INTERNAL_UNITY_REQ_T {
    Task    client_task;           /*!< Task to receive any response */
    uint16   command;
    uint16   length;   /// data's length
    uint8   data[2];
};
typedef struct AVRCP_PEER_CMD_INTERNAL_UNITY_REQ_T AVRCP_PEER_CMD_INTERNAL_UNITY_REQ;

AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* PeerMallocUnityReq(uint16 opid, int payloadSize);
AVRCP_PEER_CMD_INTERNAL_UNITY_REQ* PeerMallocUnityReqWithData(uint16 opid, int payloadSize, uint8* data);
#define PEER_MALLOC_UNITY_REQ(OPID, TYPE, DATA) PeerMallocUnityReqWithData(OPID, sizeof(TYPE),(uint8* )DATA)
#define PEER_MALLOC_UNITY_REQ_NODATA(OPID) PeerMallocUnityReqWithData(OPID, 0, NULL)

void appPeerSigSendUnityRequest(AVRCP_PEER_CMD_INTERNAL_UNITY_REQ *message);
bool appPeerSigParseUnityRequest(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
void appPeerSigParseUnityConfirm(uint16 opid, Task task, peerSigStatus status);

typedef bool (*PeerCmdParseFun)(uint8* payload);
typedef void (*PeerCmdConfirmFun)(Task task, peerSigStatus status);

typedef struct {
    uint16 command;
    uint16 payloadSize;
    PeerCmdParseFun  peerCmdParseFun;
    PeerCmdConfirmFun  peerCmdConfirmFun;
} PeerUnityParseCenter;


enum {
    AVRCP_PEER_CMD_STAROT_START = 0X10,
    AVRCP_PEER_CMD_UPGRADE_CHECK_VERSION  = AVRCP_PEER_CMD_STAROT_START,
    AVRCP_PEER_CMD_UPGRADE_ENTER,
    AVRCP_PEER_CMD_UPGRADE_EXIT,
    AVRCP_PEER_CMD_UPGRADE_CANCEL_NOTIFY_COMMIT_STATUS,

    AVRCP_PEER_CMD_SYNC_VERSION,
    AVRCP_PEER_CMD_SYNC_SN,
    /// 在此次添加
    AVRCP_PEER_CMD_STAROT_END = 0X20,
};

// endregion

// region 校验对方真实版本，用于升级提交版本  AVRCP_PEER_CMD_UPGRADE_CHECK_VERSION

typedef struct {
    uint8 softwareVersion[DEV_SWVER_LEN];
} CheckVersion;

void appPeerSigTxUpgradeCheckVersionReq(Task task, CheckVersion* checkVersion);
bool appPeerSigTxUpgradeCheckVersionParse(uint8* payload);
void appPeerSigTxUpgradeCheckVersionConfirm(Task task, peerSigStatus status);

// endregion

// region 通知Peer，app希望进入upgrade状态 AVRCP_PEER_CMD_UPGRADE_ENTER

void appPeerSigTxUpgradeEnterReq(Task task);
bool appPeerSigTxUpgradeEnterParse(uint8* payload);
void appPeerSigTxUpgradeEnterConfirm(Task task, peerSigStatus status);

// endregion

// region 通知Peer，app希望退出upgrade状态 AVRCP_PEER_CMD_UPGRADE_EXIT

void appPeerSigTxUpgradeExitReq(Task task);
bool appPeerSigTxUpgradeExitParse(uint8* payload);
void appPeerSigTxUpgradeExitConfirm(Task task, peerSigStatus status);

// endregion

// region  通知Peer，不需要发送升级状态给APP了 AVRCP_PEER_CMD_UPGRADE_CANCEL_NOTIFY_COMMIT_STATUS

void appPeerSigTxUpgradeCancelNotifyCommitStatusReq(Task task);
bool appPeerSigTxUpgradeCancelNotifyCommitStatusParse(uint8* payload);
void appPeerSigTxUpgradeCancelNotifyCommitStatusConfirm(Task task, peerSigStatus status);

// endregion

// region 同步左右耳机版本  AVRCP_PEER_CMD_SYNC_VERSION

typedef struct {
    uint8 version[DEV_HWSWVER_LEN];
} SyncVersionReq;

void appPeerSigTxSyncVersionReq(Task task);
bool appPeerSigTxSyncVersionParse(uint8* payload);
void appPeerSigTxSyncVersionConfirm(Task task, peerSigStatus status);

// endregion

// region 同步SN到对端耳机

enum {
    PEER_SN_SYNC_SENT = 1 << 0,
    PEER_SN_SYNC_RECV = 1 << 1,
    PEER_SN_SYNC_COMPLETE = PEER_SN_SYNC_SENT | PEER_SN_SYNC_RECV,
};

typedef struct {
    uint8 earbudsSN[DEV_SN_LEN];
} SyncSnReq;

void appPeerSigTxSyncSNReq(Task task);
bool appPeerSigTxSyncSNParse(uint8* payload);
void appPeerSigTxSyncSNConfirm(Task task, peerSigStatus status);


// endregion

#endif //EARBUDS1_PEER_H
