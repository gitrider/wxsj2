#include "StdAfx.h"
#include "GCCharMove.h"

using namespace Packets;
UINT GCCharMoveHandler::Execute(GCCharMove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
