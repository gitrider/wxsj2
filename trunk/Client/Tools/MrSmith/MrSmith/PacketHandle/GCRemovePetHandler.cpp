#include "StdAfx.h"
#include "GCRemovePet.h"

using namespace Packets;
UINT GCRemovePetHandler::Execute(GCRemovePet* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
