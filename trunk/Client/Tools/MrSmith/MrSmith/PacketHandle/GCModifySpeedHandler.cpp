#include "StdAfx.h"
#include "GCModifySpeed.h"

using namespace Packets;
UINT GCModifySpeedHandler::Execute(GCModifySpeed* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
