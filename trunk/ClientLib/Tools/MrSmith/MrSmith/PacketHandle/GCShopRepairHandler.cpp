#include "StdAfx.h"
#include "GCShopRepair.h"

using namespace Packets;
UINT GCShopRepairHandler::Execute(GCShopRepair* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
