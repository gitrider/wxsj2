#include "StdAfx.h"
#include "CGDoEvent.h"

using namespace Packets;
UINT CGDoEventHandler::Execute(CGDoEvent* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
