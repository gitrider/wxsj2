#include "StdAfx.h"
#include "GCStallOpen.h"

using namespace Packets;
UINT GCStallOpenHandler::Execute(GCStallOpen* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
