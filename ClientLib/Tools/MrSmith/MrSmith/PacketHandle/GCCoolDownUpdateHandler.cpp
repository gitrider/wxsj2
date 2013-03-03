#include "StdAfx.h"
#include "GCCoolDownUpdate.h"

using namespace Packets;
UINT GCCooldownUpdateHandler::Execute(GCCooldownUpdate* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
