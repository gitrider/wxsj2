#include "StdAfx.h"
#include "CGBankClose.h"

using namespace Packets;
UINT CGBankCloseHandler::Execute(CGBankClose* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
