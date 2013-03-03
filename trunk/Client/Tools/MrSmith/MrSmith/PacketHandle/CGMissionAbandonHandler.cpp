#include "StdAfx.h"
#include "CGMissionAbandon.h"

using namespace Packets;
UINT CGMissionAbandonHandler::Execute(CGMissionAbandon* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
