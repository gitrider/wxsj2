#include "StdAfx.h"
#include "GCDetailEquipList.h"

using namespace Packets;
UINT GCDetailEquipListHandler::Execute(GCDetailEquipList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
