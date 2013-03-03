#include "StdAfx.h"
#include "GCSplitItemResult.h"

using namespace Packets;
UINT GCSplitItemResultHandler::Execute(GCSplitItemResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
