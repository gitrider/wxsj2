#include "StdAfx.h"
#include "GCMissionResult.h"

using namespace Packets;
UINT GCMissionResultHandler::Execute(GCMissionResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
