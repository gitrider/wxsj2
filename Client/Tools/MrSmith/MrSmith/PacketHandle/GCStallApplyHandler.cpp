#include "StdAfx.h"
#include "GCStallApply.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"

#include "LuaPlus.h"

using namespace Packets;
UINT GCStallApplyHandler::Execute(GCStallApply* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	BYTE	IsCanStall	=	pPacket->GetIsCanStall();
	if(IsCanStall == TRUE)
	{
		pAgent->getLuaState()->DoString("Procedure_Main_SetState(\"baitan_confirm_suc\")");
	}

	return PACKET_EXE_CONTINUE;
}
