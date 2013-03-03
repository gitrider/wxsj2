#include "StdAfx.h"
#include "CGCharJump.h"

using namespace Packets;
UINT CGCharJumpHandler::Execute(CGCharJump* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
