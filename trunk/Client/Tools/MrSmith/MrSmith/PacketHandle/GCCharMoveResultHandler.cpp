#include "StdAfx.h"
#include "GCCharMoveResult.h"

using namespace Packets;
UINT GCCharMoveResultHandler::Execute(GCCharMoveResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
