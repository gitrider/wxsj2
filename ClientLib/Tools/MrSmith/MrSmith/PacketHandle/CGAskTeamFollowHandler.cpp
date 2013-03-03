#include "StdAfx.h"
#include "CGAskTeamFollow.h"

using namespace Packets;
UINT CGAskTeamFollowHandler::Execute(CGAskTeamFollow* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
