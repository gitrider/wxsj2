#include "StdAfx.h"
#include "CGExchangeCancel.h"

using namespace Packets;
UINT CGExchangeCancelHandler::Execute(CGExchangeCancel* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
