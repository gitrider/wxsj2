#include "StdAfx.h"
#include "GCPlayerCallOf.h"

using namespace Packets;
UINT GCPlayerCallOfHandler::Execute(GCPlayerCallOf* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
