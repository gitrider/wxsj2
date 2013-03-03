#include "StdAfx.h"
#include "CGBankRemoveItem.h"

using namespace Packets;
UINT CGBankRemoveItemHandler::Execute(CGBankRemoveItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
