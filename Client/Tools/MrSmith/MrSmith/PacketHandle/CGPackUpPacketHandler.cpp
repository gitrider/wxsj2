#include "StdAfx.h"
#include "CGPackUpPacket.h"

using namespace Packets;
UINT CGPackUpPacketHandler::Execute(CGPackUpPacket* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
