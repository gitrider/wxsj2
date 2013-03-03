#include "StdAfx.h"
#include "CGCharIdle.h"

using namespace Packets;
UINT CGCharIdleHandler::Execute(CGCharIdle* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
