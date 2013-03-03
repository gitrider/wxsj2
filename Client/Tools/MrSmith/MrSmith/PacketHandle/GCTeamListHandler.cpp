#include "StdAfx.h"
#include "GCTeamList.h"

using namespace Packets;
UINT GCTeamListHandler::Execute(GCTeamList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
