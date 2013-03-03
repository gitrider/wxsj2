#include "StdAfx.h"
#include "GCCityList.h"

using namespace Packets;
UINT GCCityListHandler::Execute(GCCityList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
