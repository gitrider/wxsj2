#include "StdAfx.h"
#include "CGUseGem.h"

using namespace Packets;
UINT CGUseGemHandler::Execute(CGUseGem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
