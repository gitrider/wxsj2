#include "StdAfx.h"
#include "CGUseAbility.h"

using namespace Packets;
UINT CGUseAbilityHandler::Execute(CGUseAbility* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
