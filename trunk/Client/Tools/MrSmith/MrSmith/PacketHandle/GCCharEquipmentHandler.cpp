#include "StdAfx.h"
#include "GCCharEquipment.h"

using namespace Packets;
UINT GCCharEquipmentHandler::Execute(GCCharEquipment* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
