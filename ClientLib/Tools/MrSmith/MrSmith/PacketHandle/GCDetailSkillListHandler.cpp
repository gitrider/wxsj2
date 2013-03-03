#include "StdAfx.h"
#include "GCDetailSkillList.h"

using namespace Packets;
UINT GCDetailSkillListHandler::Execute(GCDetailSkillList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
