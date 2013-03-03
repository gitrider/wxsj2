#include "StdAfx.h"
#include "CGOtherEquip.h"

using namespace Packets;
UINT CGOtherEquipHandler::Execute(CGOtherEquip* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
