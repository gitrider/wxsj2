#include "StdAfx.h"
#include "GCPacketStream.h"

using namespace Packets;
UINT GCPacketStreamHandler::Execute(GCPacketStream* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
