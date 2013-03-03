#include "StdAfx.h"
#include "GCBankRemoveItem.h"

using namespace Packets;
UINT GCBankRemoveItemHandler::Execute(GCBankRemoveItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
