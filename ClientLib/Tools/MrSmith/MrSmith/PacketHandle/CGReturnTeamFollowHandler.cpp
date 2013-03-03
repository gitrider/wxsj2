#include "StdAfx.h"
#include "CGReturnTeamFollow.h"

using namespace Packets;
UINT CGReturnTeamFollowHandler::Execute(CGReturnTeamFollow* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
