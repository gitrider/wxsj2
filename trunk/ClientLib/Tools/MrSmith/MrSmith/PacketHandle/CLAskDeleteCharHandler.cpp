#include "StdAfx.h"
#include "CLAskDeleteChar.h"

using namespace Packets;
UINT CLAskDeleteCharHandler::Execute(CLAskDeleteChar* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
