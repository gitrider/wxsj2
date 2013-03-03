#include "StdAfx.h"
#include "GCRetGMMsg.h"

using namespace Packets;
UINT GCRetGMMsgHandler::Execute(GCRetGMMsg* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
