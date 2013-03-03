#include "StdAfx.h"
#include "GCOtherAttack.h"

using namespace Packets;
UINT GCOtherAttackHandler::Execute(GCOtherAttack* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
