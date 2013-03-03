#include "StdAfx.h"
#include "GCExchangeNotifySerial.h"

using namespace Packets;
UINT GCExchangeNotifySerialHandler::Execute(GCExchangeNotifySerial* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
