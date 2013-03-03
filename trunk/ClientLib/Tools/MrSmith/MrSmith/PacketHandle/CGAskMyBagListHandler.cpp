#include "StdAfx.h"
#include "CGAskMyBagList.h"

using namespace Packets;
UINT CGAskMyBagListHandler::Execute(CGAskMyBagList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
