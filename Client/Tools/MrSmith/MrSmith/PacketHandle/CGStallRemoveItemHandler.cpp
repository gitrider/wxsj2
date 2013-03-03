#include "StdAfx.h"
#include "CGStallRemoveItem.h"

using namespace Packets;
UINT CGStallRemoveItemHandler::Execute(CGStallRemoveItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
