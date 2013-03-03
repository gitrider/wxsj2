#include "StdAfx.h"
#include "GCStopPedding.h"

using namespace Packets;
UINT GCStopPeddingHandler::Execute(GCStopPedding* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
