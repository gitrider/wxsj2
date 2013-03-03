#include "StdAfx.h"
#include "GCArrive.h"

using namespace Packets;
UINT GCArriveHandler::Execute(GCArrive* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
