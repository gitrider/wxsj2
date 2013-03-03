#include "StdAfx.h"
#include "CGAskMail.h"

using namespace Packets;
UINT CGAskMailHandler::Execute(CGAskMail* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
