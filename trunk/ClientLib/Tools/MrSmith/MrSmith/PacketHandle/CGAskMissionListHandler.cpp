#include "StdAfx.h"
#include "CGAskMissionList.h"

using namespace Packets;
UINT CGAskMissionListHandler::Execute(CGAskMissionList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
