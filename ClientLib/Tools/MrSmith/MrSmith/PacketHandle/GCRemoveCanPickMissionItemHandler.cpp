#include "StdAfx.h"
#include "GCRemoveCanPickMissionItem.h"

using namespace Packets;
UINT GCRemoveCanPickMissionItemHandler::Execute(GCRemoveCanPickMissionItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
