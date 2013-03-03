#include "StdAfx.h"
#include "GCBankAddItem.h"

using namespace Packets;
UINT GCBankAddItemHandler::Execute(GCBankAddItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
