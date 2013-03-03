#include "StdAfx.h"
#include "LCStatus.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMLog.h"
#include "LuaPlus.h"

using namespace Packets;
UINT LCStatusHandler::Execute(LCStatus* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;

	CLIENT_TURN_STATUS res = pPacket->GetClientStatus();
	switch(res)
	{
		case CTS_NORMAL:
			{
				if( pAgent->getLuaState() ) pAgent->getLuaState()->DoString("Procedure_Login_SetState(\"accnt_val_success\")");
			}
			break;

		case CTS_TURN:
			{
				if( pAgent->getLog() ) pAgent->getLog()->addLogFmt("你前面有还%d个GGMM在排队...", pPacket->GetTurnNumber());
			}
			break;
	}
	Sleep( 100 );
	return PACKET_EXE_CONTINUE;
}
