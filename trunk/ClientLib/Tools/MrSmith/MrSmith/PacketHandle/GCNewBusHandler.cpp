#include "StdAfx.h"
#include "GCNewBus.h"

using namespace Packets;
UINT GCNewBusHandler::Execute(GCNewBus* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
