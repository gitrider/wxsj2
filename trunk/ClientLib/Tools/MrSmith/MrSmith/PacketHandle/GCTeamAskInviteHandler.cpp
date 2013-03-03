#include "StdAfx.h"
#include "GCTeamAskInvite.h"

using namespace Packets;
UINT GCTeamAskInviteHandler::Execute(GCTeamAskInvite* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
