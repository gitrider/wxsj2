#include "StdAfx.h"
#include "GCBankMoney.h"

using namespace Packets;
UINT GCBankMoneyHandler::Execute(GCBankMoney* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
