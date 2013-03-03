#include "StdAfx.h"
#include "GCCharSkill_Missed.h"

using namespace Packets;
UINT GCCharSkill_MissedHandler::Execute(GCCharSkill_Missed* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
