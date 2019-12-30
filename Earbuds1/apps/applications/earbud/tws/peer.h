//
// Created by Administrator on 2019/11/11.
//

#ifndef EARBUDS1_PEER_H
#define EARBUDS1_PEER_H

#include "av_headset_av.h"

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

/////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////双击配置项////////////////////////////////////////////

/******************************************************************************
 * Peer Signalling Message Definitions
 ******************************************************************************/
#define AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG                 0x71
#define AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG_SIZE            2    /// 左+右

typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    uint8 left;
    uint8 right;
} PEER_SIG_INTERNAL_DOBULE_CLICK_CONFIG_REQ_T;

void appPeerSigTxDoubleClickConfigRequest(Task task, const bdaddr *peer_addr, uint8 left, uint8 right);
void appPeerSigHandleInternalDoubleClickConfigRequest(PEER_SIG_INTERNAL_DOBULE_CLICK_CONFIG_REQ_T *req);
bool appPeerSigHandleDoubleClickConfigCommand(AV_AVRCP_VENDOR_PASSTHROUGH_IND_T *ind);
void appPeerSigMsgDoubleClickConfigConfirmation(Task task, peerSigStatus status);

/////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////通用配置项////////////////////////////////////////////

#define AVRCP_PEER_CMD_NORMAL_CONFIG_SIZE                  6

typedef struct
{
    Task client_task;           /*!< Task to receive any response */
    uint8 apollo_config;
    uint8 wear_config;
    uint32 apollo_timestamp;
    uint32 wear_timestamp;
} PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T;

#define AVRCP_PEER_CMD_NORMAL_CONFIG                       0x72

void appPeerSigTxNormalConfigRequest(Task task, const bdaddr *peer_addr);
void appPeerSigHandleInternalNormalConfigRequest(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T *req);
bool appPeerSigHandleNormalConfigCommand(PEER_SIG_INTERNAL_NORMAL_CONFIG_REQ_T *ind);
void appPeerSigMsgNormalConfigConfirmation(Task task, peerSigStatus status);

/////////////////////////////////////////////////////////////////////////////////////////

#endif //EARBUDS1_PEER_H
