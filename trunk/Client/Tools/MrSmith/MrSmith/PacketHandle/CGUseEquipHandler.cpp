#include "StdAfx.h"
#include "CGUseEquip.h"

using namespace Packets;
UINT CGUseEquipHandler::Execute(CGUseEquip* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
