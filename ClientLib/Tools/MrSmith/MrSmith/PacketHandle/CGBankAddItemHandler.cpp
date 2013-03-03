#include "StdAfx.h"
#include "CGBankAddItem.h"

using namespace Packets;
UINT CGBankAddItemHandler::Execute(CGBankAddItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
