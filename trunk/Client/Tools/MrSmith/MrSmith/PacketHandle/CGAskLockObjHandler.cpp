#include "StdAfx.h"
#include "CGAskLockObj.h"

using namespace Packets;
UINT CGAskLockObjHandler::Execute(CGAskLockObj* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
