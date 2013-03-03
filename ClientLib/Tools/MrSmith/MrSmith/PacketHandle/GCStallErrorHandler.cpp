#include "StdAfx.h"
#include "GCStallError.h"

using namespace Packets;
UINT GCStallErrorHandler::Execute(GCStallError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
