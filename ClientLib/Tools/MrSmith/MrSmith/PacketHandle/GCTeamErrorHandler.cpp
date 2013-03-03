#include "StdAfx.h"
#include "GCTeamError.h"

using namespace Packets;
UINT GCTeamErrorHandler::Execute(GCTeamError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
