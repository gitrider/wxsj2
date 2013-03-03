#include "StdAfx.h"
#include "CGStallAddItem.h"

using namespace Packets;
UINT CGStallAddItemHandler::Execute(CGStallAddItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
