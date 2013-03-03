#include "StdAfx.h"
#include "CGBankSwapItem.h"

using namespace Packets;
UINT CGBankSwapItemHandler::Execute(CGBankSwapItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
