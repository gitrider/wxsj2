#include "StdAfx.h"
#include "GCBusAddPassenger.h"

using namespace Packets;
UINT GCBusAddPassengerHandler::Execute(GCBusAddPassenger* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
