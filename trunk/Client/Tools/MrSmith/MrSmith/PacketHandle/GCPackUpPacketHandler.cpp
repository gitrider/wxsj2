#include "StdAfx.h"
#include "GCPackUpPacket.h"

using namespace Packets;
UINT GCPackUpPacketHandler::Execute(GCPackUpPacket* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
