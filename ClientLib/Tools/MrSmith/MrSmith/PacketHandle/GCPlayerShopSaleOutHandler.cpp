#include "StdAfx.h"
#include "GCPlayerShopSaleOut.h"

using namespace Packets;
UINT GCPlayerShopSaleOutHandler::Execute(GCPlayerShopSaleOut* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
