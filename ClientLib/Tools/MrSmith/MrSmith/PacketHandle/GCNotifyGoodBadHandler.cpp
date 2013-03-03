#include "StdAfx.h"
#include "GCNotifyGoodBad.h"

using namespace Packets;
UINT GCNotifyGoodBadHandler::Execute(GCNotifyGoodBad* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
