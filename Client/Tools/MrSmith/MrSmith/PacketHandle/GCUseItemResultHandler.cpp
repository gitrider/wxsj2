#include "StdAfx.h"
#include "GCUseItemResult.h"

using namespace Packets;
UINT GCUseItemResultHandler::Execute(GCUseItemResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
