#include "StdAfx.h"
#include "GCExchangeSynchLock.h"

using namespace Packets;
UINT GCExchangeSynchLockHandler::Execute(GCExchangeSynchLock* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
