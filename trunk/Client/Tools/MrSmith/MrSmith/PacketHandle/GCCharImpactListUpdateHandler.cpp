#include "StdAfx.h"
#include "GCCharImpactListUpdate.h"

using namespace Packets;
UINT GCCharImpactListUpdateHandler::Execute(GCCharImpactListUpdate* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
