#include "StdAfx.h"
#include "CGAskTeamInfo.h"

using namespace Packets;
UINT CGAskTeamInfoHandler::Execute(CGAskTeamInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
