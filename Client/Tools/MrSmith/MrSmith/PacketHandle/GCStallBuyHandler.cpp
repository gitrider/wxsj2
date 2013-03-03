#include "StdAfx.h"
#include "GCStallBuy.h"

using namespace Packets;
UINT GCStallBuyHandler::Execute(GCStallBuy* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
