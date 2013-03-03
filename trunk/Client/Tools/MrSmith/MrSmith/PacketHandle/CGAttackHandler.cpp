#include "StdAfx.h"
#include "CGAttack.h"

using namespace Packets;
UINT CGAttackHandler::Execute(CGAttack* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
