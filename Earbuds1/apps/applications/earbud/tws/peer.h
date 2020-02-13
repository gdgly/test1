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
      PEERTX_CMD_NOTIFY_VERSION,     /* 同步左右耳机版本 */
      PEERTX_CMD_SYNC_DOUBLE_CLICK,  /* 同步双击 */
     };
void appPeerSigTxDataCommand(Task task, const bdaddr *peer_addr, uint8 command, uint16 size_payload, const uint8 *payload);
void appPeerSigTxDataCommandExt(Task task,uint8 command, uint16 size_payload, const uint8 *payload);
void appPeerSigTxDataCommandUi(uint8 command, uint8 payload);  // task为UI 仅一个字节payhload
#define appPeerSigTxWakeupSys(apo)   appPeerSigTxDataCommandUi(PEERTX_CMD_WAKEUP_SYS, (apo))
#define appPeerSigTxWakeupApp(apo)   appPeerSigTxDataCommandUi(PEERTX_CMD_WAKEUP_APP, (apo))
/*
 * 同步耳机版本信息
 */
void appPeerSigTxSyncVersion(Task task);
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
#endif //EARBUDS1_PEER_H
