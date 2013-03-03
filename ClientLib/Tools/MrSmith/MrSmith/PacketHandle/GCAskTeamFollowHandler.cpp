#include "StdAfx.h"
#include "GCAskTeamFollow.h"

using namespace Packets;
UINT GCAskTeamFollowHandler::Execute(GCAskTeamFollow* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
