#include "StdAfx.h"
#include "CGEventRequest.h"

using namespace Packets;
UINT CGEventRequestHandler::Execute(CGEventRequest* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
