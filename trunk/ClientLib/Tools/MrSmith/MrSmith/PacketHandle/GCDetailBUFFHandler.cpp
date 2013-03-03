#include "StdAfx.h"
#include "GCDetailBUFF.h"

using namespace Packets;
UINT GCDetailBuffHandler::Execute(GCDetailBuff* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
