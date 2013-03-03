#include "StdAfx.h"
#include "CGReqLevelUp.h"

using namespace Packets;
UINT CGReqLevelUpHandler::Execute(CGReqLevelUp* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
