#include "StdAfx.h"
#include "CGExpAssign.h"

using namespace Packets;
UINT CGExpAssignHandler::Execute(CGExpAssign* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
