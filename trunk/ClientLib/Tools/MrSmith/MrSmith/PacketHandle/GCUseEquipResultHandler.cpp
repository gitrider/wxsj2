#include "StdAfx.h"
#include "GCUseEquipResult.h"

using namespace Packets;
UINT GCUseEquipResultHandler::Execute(GCUseEquipResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
