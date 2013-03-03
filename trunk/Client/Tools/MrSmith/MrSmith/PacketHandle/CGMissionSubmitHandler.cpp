#include "StdAfx.h"
#include "CGMissionSubmit.h"

using namespace Packets;
UINT CGMissionSubmitHandler::Execute(CGMissionSubmit* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
