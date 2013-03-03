#include "StdAfx.h"
#include "GCDetailHealsAndDamages.h"

using namespace Packets;
UINT GCDetailHealsAndDamagesHandler::Execute(GCDetailHealsAndDamages* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
