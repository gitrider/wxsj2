#include "StdAfx.h"
#include "CGExchangeReplyI.h"

using namespace Packets;
UINT CGExchangeReplyIHandler::Execute(CGExchangeReplyI* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
