#include "StdAfx.h"
#include "GCCharAction.h"

using namespace Packets;
UINT GCCharActionHandler::Execute(GCCharAction* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
