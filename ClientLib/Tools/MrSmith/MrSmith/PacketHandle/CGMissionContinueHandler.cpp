#include "StdAfx.h"
#include "CGMissionContinue.h"

using namespace Packets;
UINT CGMissionContinueHandler::Execute(CGMissionContinue* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
