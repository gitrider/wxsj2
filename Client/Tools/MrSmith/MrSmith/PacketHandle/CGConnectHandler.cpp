#include "StdAfx.h"
#include "CGConnect.h"

using namespace Packets;
UINT CGConnectHandler::Execute(CGConnect* pPacket, Player* pAgent)
{
	return PACKET_EXE_CONTINUE;
}
