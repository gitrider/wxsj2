#include "StdAfx.h"
#include "CGMinorPasswd.h"

using namespace Packets;
UINT CGMinorPasswdHandler::Execute(CGMinorPasswd* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
