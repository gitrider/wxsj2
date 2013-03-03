#include "StdAfx.h"
#include "CGAntagonistReq.h"

using namespace Packets;
UINT CGAntagonistReqHandler::Execute(CGAntagonistReq* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
