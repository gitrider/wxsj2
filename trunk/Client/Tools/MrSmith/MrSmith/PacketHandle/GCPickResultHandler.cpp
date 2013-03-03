#include "StdAfx.h"
#include "GCPickResult.h"

using namespace Packets;
UINT GCPickResultHandler::Execute(GCPickResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
