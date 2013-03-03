#include "StdAfx.h"
#include "GCTeamMemberInfo.h"

using namespace Packets;
UINT GCTeamMemberInfoHandler::Execute(GCTeamMemberInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
