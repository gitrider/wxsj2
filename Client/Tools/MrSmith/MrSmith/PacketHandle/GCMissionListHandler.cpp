#include "StdAfx.h"
#include "GCMissionList.h"

using namespace Packets;
UINT GCMissionListHandler::Execute(GCMissionList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
