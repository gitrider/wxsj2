#include "StdAfx.h"
#include "CGStallBuy.h"

using namespace Packets;
UINT CGStallBuyHandler::Execute(CGStallBuy* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
