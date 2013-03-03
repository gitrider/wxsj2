#include "StdAfx.h"
#include "CGMissionAccept.h"

using namespace Packets;
UINT CGMissionAcceptHandler::Execute(CGMissionAccept* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
