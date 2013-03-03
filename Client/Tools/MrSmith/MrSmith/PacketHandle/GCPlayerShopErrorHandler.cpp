#include "StdAfx.h"
#include "GCPlayerShopError.h"

using namespace Packets;
UINT GCPlayerShopErrorHandler::Execute(GCPlayerShopError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
