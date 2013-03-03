#include "StdAfx.h"
#include "CGCharMove.h"

using namespace Packets;
UINT CGCharMoveHandler::Execute(CGCharMove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
