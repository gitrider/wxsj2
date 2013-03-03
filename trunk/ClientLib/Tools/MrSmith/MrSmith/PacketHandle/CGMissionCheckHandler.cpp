#include "StdAfx.h"
#include "CGMissionCheck.h"

using namespace Packets;
UINT CGMissionCheckHandler::Execute(CGMissionCheck* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
