#include "StdAfx.h"
#include "CGLegalCheck.h"

using namespace Packets;
UINT CGLegalCheckHandler::Execute(CGLegalCheck* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
