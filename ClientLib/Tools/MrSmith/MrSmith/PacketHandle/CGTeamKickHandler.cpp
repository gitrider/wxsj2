#include "StdAfx.h"
#include "CGTeamKick.h"

using namespace Packets;
UINT CGTeamKickHandler::Execute(CGTeamKick* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
