#include "StdAfx.h"
#include "GCExchangeEstablishI.h"

using namespace Packets;
UINT GCExchangeEstablishIHandler::Execute(GCExchangeEstablishI* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
