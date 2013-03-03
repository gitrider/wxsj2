#include "StdAfx.h"
#include "GCBankAcquireList.h"

using namespace Packets;
UINT GCBankAcquireListHandler::Execute(GCBankAcquireList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
