#include "StdAfx.h"
#include "GCMissionModify.h"

using namespace Packets;
UINT GCMissionModifyHandler::Execute(GCMissionModify* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
