#include "StdAfx.h"
#include "GCNewMonster_Death.h"

using namespace Packets;
UINT GCNewMonster_DeathHandler::Execute(GCNewMonster_Death* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
