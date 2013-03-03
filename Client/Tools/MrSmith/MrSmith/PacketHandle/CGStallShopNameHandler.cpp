#include "StdAfx.h"
#include "CGStallShopName.h"

using namespace Packets;
UINT CGStallShopNameHandler::Execute(CGStallShopName* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
