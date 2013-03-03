#include "StdAfx.h"
#include "GCPackage_SwapItem.h"

using namespace Packets;
UINT GCPackage_SwapItemHandler::Execute(GCPackage_SwapItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
