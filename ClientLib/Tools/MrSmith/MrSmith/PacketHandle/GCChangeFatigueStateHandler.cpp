#include "StdAfx.h"
#include "GCChangeFatigueState.h"

using namespace Packets;
UINT GCChangeFatigueStateHandler::Execute(GCChangeFatigueState* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
