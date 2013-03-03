#include "StdAfx.h"
#include "GCHeartBeat.h"

using namespace Packets;
UINT GCHeartBeatHandler::Execute(GCHeartBeat* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
