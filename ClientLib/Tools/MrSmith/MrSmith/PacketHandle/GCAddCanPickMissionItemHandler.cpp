#include "StdAfx.h"
#include "GCAddCanPickMissionItem.h"

using namespace Packets;
UINT GCAddCanPickMissionItemHandler::Execute(GCAddCanPickMissionItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
