#include "StdAfx.h"
#include "GCPlayerRealMove.h"

using namespace Packets;
UINT GCPlayerRealMoveHandler::Execute(GCPlayerRealMove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
