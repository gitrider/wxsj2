#include "StdAfx.h"
#include "GCUnEquipResult.h"

using namespace Packets;
UINT GCUnEquipResultHandler::Execute(GCUnEquipResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
