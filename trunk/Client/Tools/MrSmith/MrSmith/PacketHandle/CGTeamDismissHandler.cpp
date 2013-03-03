#include "StdAfx.h"
#include "CGTeamDismiss.h"

using namespace Packets;
UINT CGTeamDismissHandler::Execute(CGTeamDismiss* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
