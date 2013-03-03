#include "StdAfx.h"
#include "GCMinorPasswd.h"

using namespace Packets;
UINT GCMinorPasswdHandler::Execute(GCMinorPasswd* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
