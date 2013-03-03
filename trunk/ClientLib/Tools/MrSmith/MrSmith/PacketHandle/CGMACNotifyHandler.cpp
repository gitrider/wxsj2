#include "StdAfx.h"
#include "CGMACNotify.h"

using namespace Packets;
UINT CGMACNotifyHandler::Execute(CGMACNotify* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
