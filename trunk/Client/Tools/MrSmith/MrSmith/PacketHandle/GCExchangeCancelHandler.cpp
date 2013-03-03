#include "StdAfx.h"
#include "GCExchangeCancel.h"

using namespace Packets;
UINT GCExchangeCancelHandler::Execute(GCExchangeCancel* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
