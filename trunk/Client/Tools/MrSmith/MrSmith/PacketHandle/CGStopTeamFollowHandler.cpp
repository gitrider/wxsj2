#include "StdAfx.h"
#include "CGStopTeamFollow.h"

using namespace Packets;
UINT CGStopTeamFollowHandler::Execute(CGStopTeamFollow* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
