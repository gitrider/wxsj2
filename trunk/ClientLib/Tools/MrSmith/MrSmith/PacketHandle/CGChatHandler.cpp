#include "StdAfx.h"
#include "CGChat.h"

using namespace Packets;
UINT CGChatHandler::Execute(CGChat* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
