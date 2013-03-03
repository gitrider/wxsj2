#include "StdAfx.h"
#include "CGASKServerTime.h"

using namespace Packets;
UINT CGAskServerTimeHandler::Execute(CGAskServerTime* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
