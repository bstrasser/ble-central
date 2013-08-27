
#include "typedef.h"
#include "ble_hci.h"

static uint8 gapCentralRoleTaskId = 0;

static uint8  gapCentralRoleIRK[KEYLEN] = {0};
static uint8  gapCentralRoleSRK[KEYLEN] = {0};
static uint32 gapCentralRoleSignCounter = 1;
static uint8  gapCentralRoleBdAddr[B_ADDR_LEN];
static uint8  gapCentralRoleMaxScanRes = 5;
static uint16 gapCentralRoleParamId = 18;
static uint16 gapCentralRoleParamValue = 20;
static uint16 gapCentralRoleParamId1 = 19;

bStatus_t GAPCentralRole_StartDevice()
{
	GAP_DeviceInit( gapCentralRoleTaskId, GAP_PROFILE_BROADCASTER,
                         gapCentralRoleMaxScanRes, gapCentralRoleIRK,
                         gapCentralRoleSRK, &gapCentralRoleSignCounter );
  return GAP_DeviceInit( gapCentralRoleTaskId, GAP_PROFILE_CENTRAL,
                         gapCentralRoleMaxScanRes, gapCentralRoleIRK,
                         gapCentralRoleSRK, &gapCentralRoleSignCounter );
}
bStatus_t GAPCentralRole_StartDevicePerp()
{
	return GAP_DeviceInit( gapCentralRoleTaskId, GAP_PROFILE_BROADCASTER,
                         gapCentralRoleMaxScanRes, gapCentralRoleIRK,
                         gapCentralRoleSRK, &gapCentralRoleSignCounter );
}
bStatus_t GAPCentralRole_SetParam()
{
	gapParamIDs_t params;
	params = gapCentralRoleParamId1;
	GAP_SetParamValue( params, gapCentralRoleParamValue);

	params = gapCentralRoleParamId;
	//params.value = gapCentralRoleParamValue;

	return GAP_SetParamValue( params, gapCentralRoleParamValue); 
}
bStatus_t GAPCentralRole_GetParam()
{
	gapSetParam_t params;

	params.paramID = gapCentralRoleParamId;
	return GAP_GetParam( &params);
}
bStatus_t GAPCentralRole_EstablishLink( uint8 highDutyCycle, uint8 whiteList,
                                        uint8 addrTypePeer, uint8 *peerAddr )
{
  gapEstLinkReq_t params;

  params.taskID = gapCentralRoleTaskId;
  params.highDutyCycle = highDutyCycle;
  params.whiteList = whiteList;
  params.addrTypePeer = addrTypePeer;
  memcpy( params.peerAddr, peerAddr, B_ADDR_LEN );

  return GAP_EstablishLinkReq( &params );
}

bStatus_t GAPCentralRole_StartDiscovery( uint8 mode, uint8 activeScan, uint8 whiteList )
{
  gapDevDiscReq_t params;

  params.taskID = gapCentralRoleTaskId;
  params.mode = mode;
  params.activeScan = activeScan;
  params.whiteList = whiteList;

  return GAP_DeviceDiscoveryRequest( &params );
}
bStatus_t GAPCentralRole_EndDiscovery(){

	uint8 taskID = 0;

	return GAP_DeviceDiscoveryCancel(taskID);

}
bStatus_t GATTCentralRole_FindServices(){
	return GATT_DiscAllPrimaryServices();
}
bStatus_t GAPCentralRole_TerminateLink(){
	return GAP_TerminateLinkReq();
}
