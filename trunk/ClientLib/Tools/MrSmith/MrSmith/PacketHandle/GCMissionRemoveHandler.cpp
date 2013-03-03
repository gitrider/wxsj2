#include "StdAfx.h"
#include "GCMissionRemove.h"

using namespace Packets;
UINT GCMissionRemoveHandler::Execute(GCMissionRemove* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
