#include "StdAfx.h"
#include "GCCharDoAction.h"

using namespace Packets;
UINT GCCharDoActionHandler::Execute(GCCharDoAction* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
