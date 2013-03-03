#include "StdAfx.h"
#include "GCPlayerShopMoney.h"

using namespace Packets;
UINT GCPlayerShopMoneyHandler::Execute(GCPlayerShopMoney* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
