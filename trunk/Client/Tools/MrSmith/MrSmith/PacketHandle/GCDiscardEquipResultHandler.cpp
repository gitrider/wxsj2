#include "StdAfx.h"
#include "GCDiscardEquipResult.h"

using namespace Packets;
UINT GCDiscardEquipResultHandler::Execute(GCDiscardEquipResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
