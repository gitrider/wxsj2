#include "StdAfx.h"
#include "GCManualAttrResult.h"

using namespace Packets;
UINT GCManualAttrResultHandler::Execute(GCManualAttrResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
