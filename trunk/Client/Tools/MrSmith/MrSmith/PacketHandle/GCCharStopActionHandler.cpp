#include "StdAfx.h"
#include "GCCharStopAction.h"

using namespace Packets;
UINT GCCharStopActionHandler::Execute(GCCharStopAction* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
