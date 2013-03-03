#include "StdAfx.h"
#include "CLAskCharLogin.h"

using namespace Packets;
UINT CLAskCharLoginHandler::Execute(CLAskCharLogin* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
