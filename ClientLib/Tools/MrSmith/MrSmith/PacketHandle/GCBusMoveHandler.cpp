#include "StdAfx.h"
#include "GCBusMove.h"

using namespace Packets;
UINT GCBusMoveHandler::Execute(GCBusMove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
