#include "StdAfx.h"
#include "GCCharSkill_Send.h"

using namespace Packets;
UINT GCCharSkill_SendHandler::Execute(GCCharSkill_Send* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
