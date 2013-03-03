#include "StdAfx.h"
#include "GCBusStopMove.h"

using namespace Packets;
UINT GCBusStopMoveHandler::Execute(GCBusStopMove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
