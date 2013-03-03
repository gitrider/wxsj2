#include "StdAfx.h"
#include "GCCharJump.h"

using namespace Packets;
UINT GCCharJumpHandler::Execute(GCCharJump* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
