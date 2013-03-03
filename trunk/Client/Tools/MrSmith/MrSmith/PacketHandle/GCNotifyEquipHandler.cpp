#include "StdAfx.h"
#include "GCNotifyEquip.h"

using namespace Packets;
UINT GCNotifyEquipHandler::Execute(GCNotifyEquip* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
