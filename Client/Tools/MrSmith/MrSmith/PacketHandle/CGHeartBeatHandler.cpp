#include "StdAfx.h"
#include "CGHeartBeat.h"

using namespace Packets;
UINT CGHeartBeatHandler::Execute(CGHeartBeat* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
