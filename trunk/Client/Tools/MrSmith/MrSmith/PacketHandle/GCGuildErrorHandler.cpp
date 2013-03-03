#include "StdAfx.h"
#include "GCGuildError.h"

using namespace Packets;
UINT GCGuildErrorHandler::Execute(GCGuildError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
