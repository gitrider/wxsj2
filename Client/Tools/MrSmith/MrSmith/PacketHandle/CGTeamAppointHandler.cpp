#include "StdAfx.h"
#include "CGTeamAppoint.h"

using namespace Packets;
UINT CGTeamAppointHandler::Execute(CGTeamAppoint* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
