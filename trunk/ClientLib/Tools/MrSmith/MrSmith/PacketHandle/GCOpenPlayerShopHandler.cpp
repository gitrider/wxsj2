#include "StdAfx.h"
#include "GCOpenPlayerShop.h"

using namespace Packets;
UINT GCOpenPlayerShopHandler::Execute(GCOpenPlayerShop* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
