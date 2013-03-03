#include "StdAfx.h"
#include "CGAskTeamMemberInfo.h"

using namespace Packets;
UINT CGAskTeamMemberInfoHandler::Execute(CGAskTeamMemberInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
