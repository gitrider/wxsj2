#include "StdAfx.h"
#include "CGAskItemInfo.h"

using namespace Packets;
UINT CGAskItemInfoHandler::Execute(CGAskItemInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
