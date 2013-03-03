#include "StdAfx.h"
#include "CGRemoveGem.h"

using namespace Packets;
UINT CGRemoveGemHandler::Execute(CGRemoveGem* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
