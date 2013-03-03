#include "StdAfx.h"
#include "GCErrorAttack.h"

using namespace Packets;
UINT GCErrorAttackHandler::Execute(GCErrorAttack* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
