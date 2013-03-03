#include "StdAfx.h"
#include "CLAskCreateChar.h"

using namespace Packets;
UINT CLAskCreateCharHandler::Execute(CLAskCreateChar* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
