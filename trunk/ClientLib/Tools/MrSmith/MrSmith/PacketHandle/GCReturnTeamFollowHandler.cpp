#include "StdAfx.h"
#include "GCReturnTeamFollow.h"

using namespace Packets;
UINT GCReturnTeamFollowHandler::Execute(GCReturnTeamFollow* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
