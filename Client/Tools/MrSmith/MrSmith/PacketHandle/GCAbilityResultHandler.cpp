#include "StdAfx.h"
#include "GCAbilityResult.h"

using namespace Packets;
UINT GCAbilityResultHandler::Execute(GCAbilityResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
