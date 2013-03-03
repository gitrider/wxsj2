#include "StdAfx.h"
#include "GCWorldTime.h"

using namespace Packets;
UINT GCWorldTimeHandler::Execute(GCWorldTime* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
