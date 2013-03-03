#include "StdAfx.h"
#include "CLAskCharList.h"

using namespace Packets;
UINT CLAskCharListHandler::Execute(CLAskCharList* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
