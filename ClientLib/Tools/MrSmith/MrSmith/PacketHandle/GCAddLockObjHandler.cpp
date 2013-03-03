#include "StdAfx.h"
#include "GCAddLockObj.h"

using namespace Packets;
UINT GCAddLockObjHandler::Execute(GCAddLockObj* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
