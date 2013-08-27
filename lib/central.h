
#include "typedef.h"

bStatus_t GAPCentralRole_StartDevice();
bStatus_t GAPCentralRole_SetParam();
bStatus_t GAPCentralRole_GetParam();
bStatus_t GAPCentralRole_EstablishLink( uint8 highDutyCycle, uint8 whiteList,
                                        uint8 addrTypePeer, uint8 *peerAddr );
bStatus_t GAPCentralRole_StartDiscovery( uint8 mode, uint8 activeScan, uint8 whiteList );
bStatus_t GAPCentralRole_EndDiscovery();
bStatus_t GATTCentralRole_FindServices();
bStatus_t GAPCentralRole_StartDevicePerp();
bStatus_t GAPCentralRole_TerminateLink();
