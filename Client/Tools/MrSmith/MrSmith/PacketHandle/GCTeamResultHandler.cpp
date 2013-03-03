#include "StdAfx.h"
#include "GCTeamResult.h"

using namespace Packets;
UINT GCTeamResultHandler::Execute(GCTeamResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
