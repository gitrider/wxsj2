#include "StdAfx.h"
#include "GCAttack.h"

using namespace Packets;
UINT GCAttackHandler::Execute(GCAttack* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
