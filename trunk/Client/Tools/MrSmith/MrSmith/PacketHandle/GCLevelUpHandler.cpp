#include "StdAfx.h"
#include "GCLevelUp.h"

using namespace Packets;
UINT GCLevelUpHandler::Execute(GCLevelUp* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
