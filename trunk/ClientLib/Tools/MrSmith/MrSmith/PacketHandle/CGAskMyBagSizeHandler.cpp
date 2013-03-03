#include "StdAfx.h"
#include "CGAskMyBagSize.h"

using namespace Packets;
UINT CGAskMyBagSizeHandler::Execute(CGAskMyBagSize* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
