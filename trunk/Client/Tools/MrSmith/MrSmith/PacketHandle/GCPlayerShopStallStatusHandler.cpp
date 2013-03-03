#include "StdAfx.h"
#include "GCPlayerShopStallStatus.h"

using namespace Packets;
UINT GCPlayerShopStallStatusHandler::Execute(GCPlayerShopStallStatus* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
