#include "StdAfx.h"
#include "GCGuild.h"

using namespace Packets;
UINT GCGuildHandler::Execute(GCGuild* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
