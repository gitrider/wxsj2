#include "StdAfx.h"
#include "GCItemList.h"

using namespace Packets;
UINT GCItemListHandler::Execute(GCItemList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
