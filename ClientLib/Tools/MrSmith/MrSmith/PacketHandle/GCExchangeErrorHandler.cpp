#include "StdAfx.h"
#include "GCExchangeError.h"

using namespace Packets;
UINT GCExchangeErrorHandler::Execute(GCExchangeError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
