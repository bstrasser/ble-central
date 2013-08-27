
#ifndef BLE_HCI_H
#define BLE_HCI_H

#include "typedef.h"

#if defined(__AVR_ATmega328P__)
#include <AltSoftSerial.h>
uint8 ble_hci_init(AltSoftSerial *ser);
#else
uint8 ble_hci_init();
#endif

bStatus_t GAP_DeviceInit( uint8 taskID, uint8 profileRole, uint8 maxScanResponses, uint8 *pIRK, uint8 *pSRK, uint32 *pSignCounter );
//bStatus_t GAP_SetParam( gapSetParam_t *pParams);
bStatus_t GAP_SetParamValue	( gapParamIDs_t paramID, uint16 paramValue);	
bStatus_t GAP_GetParam( gapSetParam_t *pParams);
bStatus_t GAP_DeviceDiscoveryRequest( gapDevDiscReq_t *pParams );
bStatus_t GAP_DeviceDiscoveryCancel (uint8 	taskID);
bStatus_t GAP_TerminateLinkReq ();
bStatus_t GAP_EstablishLinkReq( gapEstLinkReq_t *pParams );
bStatus_t GATT_WriteCharValue( uint16 connHandle, attWriteReq_t *pReq, uint8 taskId );
bStatus_t GATT_WriteNoRsp( uint16 connHandle, attWriteReq_t *pReq, uint8 taskId );
bStatus_t GATT_DiscAllPrimaryServices();
#endif /* BLE_HCI_H */

