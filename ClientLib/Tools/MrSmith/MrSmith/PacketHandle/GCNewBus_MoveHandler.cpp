#include "StdAfx.h"
#include "GCNewBus_Move.h"

using namespace Packets;
UINT GCNewBus_MoveHandler::Execute(GCNewBus_Move* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
