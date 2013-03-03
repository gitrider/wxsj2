#include "StdAfx.h"
#include "GCShopSoldList.h"

using namespace Packets;
UINT GCShopSoldListHandler::Execute(GCShopSoldList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
