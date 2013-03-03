#include "StdAfx.h"
#include "GCNewPet_Move.h"

using namespace Packets;
UINT GCNewPet_MoveHandler::Execute(GCNewPet_Move* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
