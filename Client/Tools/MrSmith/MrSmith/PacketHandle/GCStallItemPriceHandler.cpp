#include "StdAfx.h"
#include "GCStallItemPrice.h"

using namespace Packets;
UINT GCStallItemPriceHandler::Execute(GCStallItemPrice* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
