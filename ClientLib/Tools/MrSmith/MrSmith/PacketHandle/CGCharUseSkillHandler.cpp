#include "StdAfx.h"
#include "CGCharUseSkill.h"

using namespace Packets;
UINT CGCharUseSkillHandler::Execute(CGCharUseSkill* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
