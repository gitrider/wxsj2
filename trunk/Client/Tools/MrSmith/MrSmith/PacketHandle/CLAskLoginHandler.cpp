#include "StdAfx.h"
#include "CLAskLogin.h"

using namespace Packets;
UINT CLAskLoginHandler::Execute(CLAskLogin* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
