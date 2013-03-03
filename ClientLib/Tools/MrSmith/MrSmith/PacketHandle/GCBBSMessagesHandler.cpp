#include "StdAfx.h"
#include "GCBBSMessages.h"

using namespace Packets;
UINT GCBBSMessagesHandler::Execute(GCBBSMessages* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
