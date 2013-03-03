#include "StdAfx.h"
#include "CGWaiguaInfo.h"

using namespace Packets;
UINT CGWaiguaInfoHandler::Execute(CGWaiguaInfo* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
