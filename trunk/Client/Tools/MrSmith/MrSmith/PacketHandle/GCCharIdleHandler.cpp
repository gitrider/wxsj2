#include "StdAfx.h"
#include "GCCharIdle.h"

using namespace Packets;
UINT GCCharIdleHandler::Execute(GCCharIdle* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
