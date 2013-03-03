#include "StdAfx.h"
#include "GCGuildReturn.h"

using namespace Packets;
UINT GCGuildReturnHandler::Execute(GCGuildReturn* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
