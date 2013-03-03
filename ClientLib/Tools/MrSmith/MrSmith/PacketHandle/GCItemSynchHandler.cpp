#include "StdAfx.h"
#include "GCItemSynch.h"

using namespace Packets;
UINT GCItemSynchHandler::Execute(GCItemSynch* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
