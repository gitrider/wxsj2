#include "StdAfx.h"
#include "CGExecuteScript.h"

using namespace Packets;
UINT CGExecuteScriptHandler::Execute(CGExecuteScript* pPacket, Player*)
{
	return PACKET_EXE_CONTINUE;
}
