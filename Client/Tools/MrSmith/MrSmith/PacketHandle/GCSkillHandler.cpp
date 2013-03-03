#include "StdAfx.h"
#include "GCSkill.h"

using namespace Packets;
UINT GCSkillHandler::Execute(GCSkill* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
