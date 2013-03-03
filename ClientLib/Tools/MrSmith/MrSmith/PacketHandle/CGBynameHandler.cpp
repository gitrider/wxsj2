#include "StdAfx.h"
#include "CGByname.h"

using namespace Packets;
UINT CGBynameHandler::Execute(CGByname* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
