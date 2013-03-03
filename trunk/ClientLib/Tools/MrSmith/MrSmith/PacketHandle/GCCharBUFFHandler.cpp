#include "StdAfx.h"
#include "GCCharBUFF.h"

using namespace Packets;
UINT GCCharBuffHandler::Execute(GCCharBuff* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
