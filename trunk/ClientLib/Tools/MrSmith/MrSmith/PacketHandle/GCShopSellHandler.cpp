#include "StdAfx.h"
#include "GCShopSell.h"

using namespace Packets;
UINT GCShopSellHandler::Execute(GCShopSell* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
