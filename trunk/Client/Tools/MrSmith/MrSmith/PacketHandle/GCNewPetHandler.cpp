#include "StdAfx.h"
#include "GCNewPet.h"

using namespace Packets;
UINT GCNewPetHandler::Execute(GCNewPet* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
