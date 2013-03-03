#include "StdAfx.h"
#include "CGUnEquip.h"

using namespace Packets;
UINT CGUnEquipHandler::Execute(CGUnEquip* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
