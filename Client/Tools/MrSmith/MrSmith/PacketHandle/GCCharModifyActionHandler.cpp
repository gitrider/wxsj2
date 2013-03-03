#include "StdAfx.h"
#include "GCCharModifyAction.h"

using namespace Packets;
UINT GCCharModifyActionHandler::Execute(GCCharModifyAction* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
