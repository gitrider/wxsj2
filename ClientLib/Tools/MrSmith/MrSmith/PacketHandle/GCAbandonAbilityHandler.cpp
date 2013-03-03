#include "StdAfx.h"
#include "GCAbandonAbility.h"

using namespace Packets;
UINT GCAbandonAbilityHandler::Execute(GCAbandonAbility* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
