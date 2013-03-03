#include "StdAfx.h"
#include "GCMyBagList.h"

using namespace Packets;
UINT GCMyBagListHandler::Execute(GCMyBagList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
