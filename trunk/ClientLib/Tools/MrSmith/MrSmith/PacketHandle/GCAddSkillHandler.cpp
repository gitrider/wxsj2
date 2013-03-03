#include "StdAfx.h"
#include "GCAddSkill.h"

using namespace Packets;
UINT GCAddSkillHandler::Execute(GCAddSkill* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
