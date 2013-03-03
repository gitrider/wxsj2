#include "StdAfx.h"
#include "CGMail.h"

using namespace Packets;
UINT CGMailHandler::Execute(CGMail* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
