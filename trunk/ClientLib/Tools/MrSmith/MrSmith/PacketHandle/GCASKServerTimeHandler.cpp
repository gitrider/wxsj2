#include "StdAfx.h"
#include "GCASKServerTime.h"

using namespace Packets;
UINT GCAskServerTimeHandler::Execute(GCAskServerTime* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
