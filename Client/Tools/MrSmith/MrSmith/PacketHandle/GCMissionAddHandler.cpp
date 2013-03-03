#include "StdAfx.h"
#include "GCMissionAdd.h"

using namespace Packets;
UINT GCMissionAddHandler::Execute(GCMissionAdd* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
