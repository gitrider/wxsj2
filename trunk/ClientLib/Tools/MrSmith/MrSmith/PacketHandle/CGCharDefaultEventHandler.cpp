#include "StdAfx.h"
#include "CGCharDefaultEvent.h"

using namespace Packets;
UINT CGCharDefaultEventHandler::Execute(CGCharDefaultEvent* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
