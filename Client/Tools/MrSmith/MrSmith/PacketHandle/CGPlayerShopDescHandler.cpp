#include "StdAfx.h"
#include "CGPlayerShopDesc.h"

using namespace Packets;
UINT CGPlayerShopDescHandler::Execute(CGPlayerShopDesc* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
