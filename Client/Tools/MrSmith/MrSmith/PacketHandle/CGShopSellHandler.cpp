#include "StdAfx.h"
#include "CGShopSell.h"

using namespace Packets;
UINT CGShopSellHandler::Execute(CGShopSell* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
