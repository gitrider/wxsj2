#include "StdAfx.h"
#include "CGLockTarget.h"

using namespace Packets;
UINT CGLockTargetHandler::Execute(CGLockTarget* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
