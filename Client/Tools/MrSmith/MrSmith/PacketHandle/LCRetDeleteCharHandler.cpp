#include "StdAfx.h"
#include "LCRetDeleteChar.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "LuaPlus.h"

using namespace Packets;
UINT LCRetDeleteCharHandler::Execute(LCRetDeleteChar* pPacket, Player* pPlayer)
{
	return PACKET_EXE_CONTINUE;
}
