#include "StdAfx.h"
#include "GCAbilityAction.h"

using namespace Packets;
UINT GCAbilityActionHandler::Execute(GCAbilityAction* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
