#include "StdAfx.h"
#include "GCMissionHaveDoneFlag.h"

using namespace Packets;
UINT GCMissionHaveDoneFlagHandler::Execute(GCMissionHaveDoneFlag* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
