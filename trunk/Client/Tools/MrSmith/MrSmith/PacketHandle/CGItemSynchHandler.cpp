#include "StdAfx.h"
#include "CGItemSynch.h"

using namespace Packets;
UINT CGItemSynchHandler::Execute(CGItemSynch* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
