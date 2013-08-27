
#include "typedef.h"
#include "ble_hci.h"

#if defined(__AVR_ATmega328P__)
  #include <AltSoftSerial.h>
  AltSoftSerial *TX;
  uint8 ble_hci_init(AltSoftSerial *ser)
  {
    TX = ser;
  }
#else
  uint8 ble_hci_init()
  {
  }
#endif

bStatus_t GAP_DeviceInit( uint8 taskID, uint8 profileRole, uint8 maxScanResponses, uint8 *pIRK, uint8 *pSRK, uint32 *pSignCounter )
{
  uint8 buf[42];
  uint8 len = 0;
    
  buf[len++] = 0x01;                  // -Type    : 0x01 (Command)
  buf[len++] = 0x00;                  // -Opcode  : 0xFE00 (GAP_DeviceInit)
  buf[len++] = 0xFE;
  
  buf[len++] = 0x26;                  // -Data Length
  buf[len++] = profileRole;           //  Profile Role
  buf[len++] = maxScanResponses;      //  MaxScanRsps
  memcpy(&buf[len], pIRK, 16);        //  IRK
  len += 16;
  memcpy(&buf[len], pSRK, 16);        //  SRK
  len += 16;
  memcpy(&buf[len], pSignCounter, 4); //  SignCounter
  len += 4;

#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}
bStatus_t GAP_DeviceDiscoveryCancel (uint8 	taskID){
  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;                  // -Type    : 0x01 (Command)
  buf[len++] = 0x05;                  // -Opcode  : 0xFE00 (GAP_DeviceInit)
  buf[len++] = 0xFE;

  buf[len++] = 0x01;
  buf[len++] = taskID;

  #if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
  #else
  Serial1.write(buf, len);
  #endif

  return 1;

}
bStatus_t GAP_GetParam( gapSetParam_t *pParams)
{
  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;                  // -Type    : 0x01 (Command)
   memcpy(&buf[len], "\x31\xFE", 2);
  len += 2;
 
  buf[len++] = 0x02;                 // -Data Length
  memcpy(&buf[len], &pParams->paramID, 2);        
  len +=2;
  
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}

bStatus_t GAP_SetParamValue( gapParamIDs_t paramID, uint16 paramValue)
{
  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;                  // -Type    : 0x01 (Command)
   memcpy(&buf[len], "\x30\xFE", 2);
  len += 2;
 
  buf[len++] = 0x04;                 // -Data Length
//  buf[len++] = pParams->paramID;        //  ID
  memcpy(&buf[len], &paramID, 2);        
  len +=2;
  memcpy(&buf[len], &paramValue, 2);        //  value
  len +=2;
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}
bStatus_t GAP_DeviceDiscoveryRequest( gapDevDiscReq_t *pParams )
{
  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;                  // -Type    : 0x01 (Command)
  buf[len++] = 0x04;                  // -Opcode  : 0xFE04 (GAP_DeviceDiscoveryRequest)
  buf[len++] = 0xFE;
    
  buf[len++] = 0x03;                 // -Data Length
  buf[len++] = pParams->mode;        //  Mode
  buf[len++] = pParams->activeScan;  //  ActiveScan
  buf[len++] = pParams->whiteList;   //  WhiteList
  
//  hci_tl_write(buf, len);
//  hci_tl_wait_for_response();
  
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}

bStatus_t GAP_EstablishLinkReq( gapEstLinkReq_t *pParams )
{
  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;
  memcpy(&buf[len], "\x09\xFE", 2);
  len += 2;
  
  buf[len++] = 0x03 + B_ADDR_LEN;
  
  buf[len++] = pParams->highDutyCycle;
  buf[len++] = pParams->whiteList;    
  buf[len++] = pParams->addrTypePeer;
  memcpy(&buf[len], pParams->peerAddr, B_ADDR_LEN);
  len+=B_ADDR_LEN;

#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}
bStatus_t GAP_TerminateLinkReq ()
{

  uint8 buf[20];
  uint8 len = 0;
    
  buf[len++] = 0x01;
  memcpy(&buf[len], "\x0A\xFE", 2);
  len += 2;
   buf[len++] = 0x02;
 
  memcpy(&buf[len], "\x00\x00", 2);
  len += 2;
  
  
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

return 1;
  
}
bStatus_t GATT_WriteCharValue( uint16 connHandle, attWriteReq_t *pReq, uint8 taskId )
{
  uint8 buf[20];
  uint8 len = 0;
  
  buf[len++] = 0x01;
  memcpy(&buf[len], "\x92\xFD", 2);
  len += 2;
  
  buf[len++] = 0x04 + pReq->len;
  
  memcpy(&buf[len], &connHandle, 2);
  len += 2;
  memcpy(&buf[len], &pReq->handle, 2);
  len += 2;
  memcpy(&buf[len], pReq->value, pReq->len);
  len += pReq->len;
  
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}

bStatus_t GATT_WriteNoRsp( uint16 connHandle, attWriteReq_t *pReq, uint8 taskId )
{
  uint8 buf[20];
  uint8 len = 0;
  
  buf[len++] = 0x01;
  memcpy(&buf[len], "\xB6\xFD", 2);
  len += 2;
  
  buf[len++] = 0x04 + pReq->len;
  
  memcpy(&buf[len], &connHandle, 2);
  len += 2;
  memcpy(&buf[len], &pReq->handle, 2);
  len += 2;
  memcpy(&buf[len], pReq->value, pReq->len);
  len += pReq->len;
  
#if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}
bStatus_t GATT_DiscAllPrimaryServices(){
  uint8 buf[256];
  uint8 len = 0;
  
  buf[len++] = 0x01;
  memcpy(&buf[len], "\x88\xFD", 2);
  len += 2;
  
  memcpy(&buf[len], "\x00\x00", 2);
  len += 2;
  
  memcpy(&buf[len], "\xFF\xFF", 2);
  len += 2;
  uint8 temp[16]={0xE7, 0x88, 0xD7, 0x3B, 0xE7, 0x93, 0x4D, 0x9E, 0xA6, 0x08, 0x2F, 0x2B, 0xAF, 0xC5, 0x9A, 0x00};
  memcpy(&buf[len], &temp, 2);
  len += 16;


  #if defined(__AVR_ATmega328P__)
  (*TX).write(buf, len);
#else
  Serial1.write(buf, len);
#endif

  return 1;
}
/*
bStatus_t GAP_TerminateLinkReq( uint8 taskID, uint16 connectionHandle )
{
}
*/


