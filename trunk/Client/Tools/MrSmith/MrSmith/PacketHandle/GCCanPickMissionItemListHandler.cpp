#include "StdAfx.h"
#include "GCCanPickMissionItemList.h"

using namespace Packets;
UINT GCCanPickMissionItemListHandler::Execute(GCCanPickMissionItemList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
