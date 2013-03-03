#include "StdAfx.h"
#include "CGShopClose.h"

using namespace Packets;
UINT CGShopCloseHandler::Execute(CGShopClose* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
