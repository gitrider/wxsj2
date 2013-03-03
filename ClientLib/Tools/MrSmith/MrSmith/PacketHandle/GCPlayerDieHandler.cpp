#include "StdAfx.h"
#include "GCPlayerDie.h"

using namespace Packets;
UINT GCPlayerDieHandler::Execute(GCPlayerDie* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
