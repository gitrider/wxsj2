#include "StdAfx.h"
#include "GCDie.h"

using namespace Packets;
UINT GCDieHandler::Execute(GCDie* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
