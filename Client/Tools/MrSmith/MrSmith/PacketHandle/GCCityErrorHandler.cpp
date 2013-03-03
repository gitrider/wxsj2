#include "StdAfx.h"
#include "GCCityError.h"

using namespace Packets;
UINT GCCityErrorHandler::Execute(GCCityError* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
