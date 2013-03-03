#include "StdAfx.h"
#include "GCDetailImpactListUpdate.h"

using namespace Packets;
UINT GCDetailImpactListUpdateHandler::Execute(GCDetailImpactListUpdate* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
