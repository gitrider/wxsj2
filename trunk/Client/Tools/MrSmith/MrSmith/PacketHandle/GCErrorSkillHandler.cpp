#include "StdAfx.h"
#include "GCErrorSkill.h"

using namespace Packets;
UINT GCErrorSkillHandler::Execute(GCErrorSkill* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
