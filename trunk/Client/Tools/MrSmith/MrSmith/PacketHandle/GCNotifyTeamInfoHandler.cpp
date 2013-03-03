#include "StdAfx.h"
#include "GCNotifyTeamInfo.h"

using namespace Packets;
UINT GCNotifyTeamInfoHandler::Execute(GCNotifyTeamInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
