#include "StdAfx.h"
#include "GCRemoveGemResult.h"

using namespace Packets;
UINT GCRemoveGemResultHandler::Execute(GCRemoveGemResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
