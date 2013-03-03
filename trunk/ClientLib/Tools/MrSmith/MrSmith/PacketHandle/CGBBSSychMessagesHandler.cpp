#include "StdAfx.h"
#include "CGBBSSychMessages.h"

using namespace Packets;
UINT CGBBSSychMessagesHandler::Execute(CGBBSSychMessages* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
