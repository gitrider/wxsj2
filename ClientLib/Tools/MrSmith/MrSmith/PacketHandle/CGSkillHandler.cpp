#include "StdAfx.h"
#include "CGSkill.h"

using namespace Packets;
UINT CGSkillHandler::Execute(CGSkill* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
