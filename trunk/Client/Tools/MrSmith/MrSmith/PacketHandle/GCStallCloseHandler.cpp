#include "StdAfx.h"
#include "GCStallClose.h"

using namespace Packets;
UINT GCStallCloseHandler::Execute(GCStallClose* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
