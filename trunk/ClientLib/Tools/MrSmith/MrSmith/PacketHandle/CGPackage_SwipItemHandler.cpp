#include "StdAfx.h"
#include "CGPackage_SwapItem.h"

using namespace Packets;
UINT CGPackage_SwapItemHandler::Execute(CGPackage_SwapItem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
