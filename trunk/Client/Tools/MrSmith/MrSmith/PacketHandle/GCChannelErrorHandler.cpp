#include "StdAfx.h"
#include "GCChannelError.h"

using namespace Packets;
UINT GCChannelErrorHandler::Execute(GCChannelError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
