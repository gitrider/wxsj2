#include "StdAfx.h"
#include "GCBoxItemList.h"

using namespace Packets;
UINT GCBoxItemListHandler::Execute(GCBoxItemList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
