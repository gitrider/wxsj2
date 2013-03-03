#include "StdAfx.h"
#include "CGCharStopLogic.h"

using namespace Packets;
UINT CGCharStopLogicHandler::Execute(CGCharStopLogic* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
