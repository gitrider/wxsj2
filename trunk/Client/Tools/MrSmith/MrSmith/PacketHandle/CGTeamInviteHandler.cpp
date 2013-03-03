#include "StdAfx.h"
#include "CGTeamInvite.h"

using namespace Packets;
UINT CGTeamInviteHandler::Execute(CGTeamInvite* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
