#include "StdAfx.h"
#include "GCDiscardItemResult.h"

using namespace Packets;
UINT GCDiscardItemResultHandler::Execute(GCDiscardItemResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
