#include "StdAfx.h"
#include "CGPlayerShopName.h"

using namespace Packets;
UINT CGPlayerShopNameHandler::Execute(CGPlayerShopName* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
