#include "StdAfx.h"
#include "CGDispelBuffReq.h"

using namespace Packets;
UINT CGDispelBuffReqHandler::Execute(CGDispelBuffReq* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
