#include "StdAfx.h"
#include "CGTeamLeave.h"

using namespace Packets;
UINT CGTeamLeaveHandler::Execute(CGTeamLeave* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
