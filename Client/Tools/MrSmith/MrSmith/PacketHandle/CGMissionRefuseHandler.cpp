#include "StdAfx.h"
#include "CGMissionRefuse.h"

using namespace Packets;
UINT CGMissionRefuseHandler::Execute(CGMissionRefuse* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
