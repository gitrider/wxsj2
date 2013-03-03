#include "StdAfx.h"
#include "GCExpAssign.h"

using namespace Packets;
UINT GCExpAssignHandler::Execute(GCExpAssign* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
