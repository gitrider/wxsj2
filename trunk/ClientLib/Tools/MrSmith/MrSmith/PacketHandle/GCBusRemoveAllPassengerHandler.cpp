#include "StdAfx.h"
#include "GCBusRemoveAllPassenger.h"

using namespace Packets;
UINT GCBusRemoveAllPassengerHandler::Execute(GCBusRemoveAllPassenger* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
