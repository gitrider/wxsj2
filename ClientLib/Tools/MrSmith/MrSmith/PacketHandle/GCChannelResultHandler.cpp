#include "StdAfx.h"
#include "GCChannelResult.h"

using namespace Packets;
UINT GCChannelResultHandler::Execute(GCChannelResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
