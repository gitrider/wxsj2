#include "StdAfx.h"
#include "GCReqResetAttrResult.h"

using namespace Packets;
UINT GCReqResetAttrResultHandler::Execute(GCReqResetAttrResult* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
