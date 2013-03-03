#include "StdAfx.h"
#include "GCCityNotify.h"

using namespace Packets;
UINT GCCityNotifyHandler::Execute(GCCityNotify* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
