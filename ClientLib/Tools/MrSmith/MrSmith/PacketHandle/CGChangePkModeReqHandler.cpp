#include "StdAfx.h"
#include "CGChangePkModeReq.h"

using namespace Packets;
UINT CGChangePkModeReqHandler::Execute(CGChangePkModeReq* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
