#include "StdAfx.h"
#include "CGDiscardEquip.h"

using namespace Packets;
UINT CGDiscardEquipHandler::Execute(CGDiscardEquip* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
