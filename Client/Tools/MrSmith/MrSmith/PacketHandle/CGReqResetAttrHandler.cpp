#include "StdAfx.h"
#include "CGReqResetAttr.h"

using namespace Packets;
UINT CGReqResetAttrHandler::Execute(CGReqResetAttr* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
