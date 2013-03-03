#include "StdAfx.h"
#include "LCRetConnect.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "LuaPlus.h"

using namespace Packets;
UINT LCRetConnectHandler::Execute(LCRetConnect* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	// set login procedure status.
	pAgent->getLuaState()->DoString("Procedure_Login_SetState(\"conn_val_success\")");

	return PACKET_EXE_CONTINUE;
}
