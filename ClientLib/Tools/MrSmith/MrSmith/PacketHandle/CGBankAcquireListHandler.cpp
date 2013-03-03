#include "StdAfx.h"
#include "CGBankAcquireList.h"

using namespace Packets;
UINT CGBankAcquireListHandler::Execute(CGBankAcquireList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
