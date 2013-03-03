#include "StdAfx.h"
#include "CLAskCreateCode.h"

using namespace Packets;
UINT CLAskCreateCodeHandler::Execute(CLAskCreateCode* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
