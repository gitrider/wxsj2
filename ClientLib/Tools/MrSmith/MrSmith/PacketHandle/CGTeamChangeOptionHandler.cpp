#include "StdAfx.h"
#include "CGTeamChangeOption.h"

using namespace Packets;
UINT CGTeamChangeOptionHandler::Execute(CGTeamChangeOption* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
