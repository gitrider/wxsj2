#include "StdAfx.h"
#include "GCTeamFollowErr.h"

using namespace Packets;
UINT GCTeamFollowErrHandler::Execute(GCTeamFollowErr* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
