#include "StdAfx.h"
#include "GCObjTeleport.h"

using namespace Packets;
UINT GCObjTeleportHandler::Execute(GCObjTeleport* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
