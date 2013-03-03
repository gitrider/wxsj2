#include "StdAfx.h"
#include "GCNewMonster_Move.h"

using namespace Packets;
UINT GCNewMonster_MoveHandler::Execute(GCNewMonster_Move* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
