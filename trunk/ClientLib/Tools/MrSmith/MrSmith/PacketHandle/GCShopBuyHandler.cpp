#include "StdAfx.h"
#include "GCShopBuy.h"

using namespace Packets;
UINT GCShopBuyHandler::Execute(GCShopBuy* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
