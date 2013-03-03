#include "StdAfx.h"
#include "GCTargetListAndHitFlags.h"

using namespace Packets;
UINT GCTargetListAndHitFlagsHandler::Execute(GCTargetListAndHitFlags* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
