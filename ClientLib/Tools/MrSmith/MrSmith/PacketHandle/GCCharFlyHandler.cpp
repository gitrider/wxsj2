#include "StdAfx.h"
#include "GCCharFly.h"

using namespace Packets;
UINT GCCharFlyHandler::Execute(GCCharFly* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
