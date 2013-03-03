#include "StdAfx.h"
#include "CGAskPrivateInfo.h"

using namespace Packets;
UINT CGAskPrivateInfoHandler::Execute(CGAskPrivateInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
