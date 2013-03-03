#include "StdAfx.h"
#include "CGGuildJoin.h"

using namespace Packets;
UINT CGGuildJoinHandler::Execute(CGGuildJoin* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
