#include "StdAfx.h"
#include "GCTeamOptionChanged.h"

using namespace Packets;
UINT GCTeamOptionChangedHandler::Execute(GCTeamOptionChanged* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
