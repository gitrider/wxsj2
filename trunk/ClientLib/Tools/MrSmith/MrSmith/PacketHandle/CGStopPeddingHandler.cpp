#include "StdAfx.h"
#include "CGStopPedding.h"

using namespace Packets;
UINT CGStopPeddingHandler::Execute(CGStopPedding* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
