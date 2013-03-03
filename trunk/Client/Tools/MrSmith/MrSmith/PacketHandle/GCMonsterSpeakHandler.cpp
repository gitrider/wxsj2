#include "StdAfx.h"
#include "GCMonsterSpeak.h"

using namespace Packets;
UINT GCMonsterSpeakHandler::Execute(GCMonsterSpeak* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
