#include "StdAfx.h"
#include "GCBankSwapItem.h"

using namespace Packets;
UINT GCBankSwapItemHandler::Execute(GCBankSwapItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
