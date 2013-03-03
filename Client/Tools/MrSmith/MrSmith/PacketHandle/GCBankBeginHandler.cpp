#include "StdAfx.h"
#include "GCBankBegin.h"

using namespace Packets;
UINT GCBankBeginHandler::Execute(GCBankBegin* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
