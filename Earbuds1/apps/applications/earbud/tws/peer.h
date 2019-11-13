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
#define AVRCP_PEER_CMD_BLE_CONFIG                 0xA0
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
#define AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG                 0xA1
#define AVRCP_PEER_CMD_DOUBLE_CLICK_CONFIG_SIZE            2    /// 左+有

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



#endif //EARBUDS1_PEER_H
