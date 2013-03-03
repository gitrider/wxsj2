#include "StdAfx.h"
#include "GCBusRemovePassenger.h"

using namespace Packets;
UINT GCBusRemovePassengerHandler::Execute(GCBusRemovePassenger* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
