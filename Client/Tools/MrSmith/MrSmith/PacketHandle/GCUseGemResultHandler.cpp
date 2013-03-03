#include "StdAfx.h"
#include "GCUseGemResult.h"

using namespace Packets;
UINT GCUseGemResultHandler::Execute(GCUseGemResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
