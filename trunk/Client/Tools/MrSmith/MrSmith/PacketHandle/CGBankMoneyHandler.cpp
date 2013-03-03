#include "StdAfx.h"
#include "CGBankMoney.h"

using namespace Packets;
UINT CGBankMoneyHandler::Execute(CGBankMoney* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
