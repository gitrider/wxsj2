#include "StdAfx.h"
#include "CGExchangeSynchLock.h"

using namespace Packets;
UINT CGExchangeSynchLockHandler::Execute(CGExchangeSynchLock* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
