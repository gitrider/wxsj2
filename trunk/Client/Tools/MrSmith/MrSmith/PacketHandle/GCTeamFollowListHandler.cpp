#include "StdAfx.h"
#include "GCTeamFollowList.h"

using namespace Packets;
UINT GCTeamFollowListHandler::Execute(GCTeamFollowList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
