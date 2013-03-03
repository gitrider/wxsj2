#include "StdAfx.h"
#include "CGSetMoodToHead.h"

using namespace Packets;
UINT CGSetMoodToHeadHandler::Execute(CGSetMoodToHead* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
