#include "StdAfx.h"
#include "GCNewPet_Death.h"

using namespace Packets;
UINT GCNewPet_DeathHandler::Execute(GCNewPet_Death* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
