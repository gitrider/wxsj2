#include "StdAfx.h"
#include "GCStallAddItem.h"

using namespace Packets;
UINT GCStallAddItemHandler::Execute(GCStallAddItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
