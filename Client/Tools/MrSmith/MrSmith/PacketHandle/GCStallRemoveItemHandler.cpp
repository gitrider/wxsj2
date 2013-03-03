#include "StdAfx.h"
#include "GCStallRemoveItem.h"

using namespace Packets;
UINT GCStallRemoveItemHandler::Execute(GCStallRemoveItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
