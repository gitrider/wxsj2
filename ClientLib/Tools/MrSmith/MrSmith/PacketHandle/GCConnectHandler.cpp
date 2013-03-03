#include "StdAfx.h"
#include "GCConnect.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "LuaPlus.h"
#include "../SMVariable.h"

using namespace Packets;
UINT GCConnectHandler::Execute(GCConnect* pPacket, Player* pPlayer)
{
	SMITH::Agent* pAgent = (SMITH::Agent*)pPlayer;
	//save variable
	pAgent->getVariable()->setAs_Int("Scene_ID", (INT)pPacket->GetSceneID());
	pAgent->getVariable()->setAs_Vector2("Scene_EnterPos", 
				(float)pPacket->GetWorldPos()->m_fX, (float)pPacket->GetWorldPos()->m_fZ);


	pAgent->getLuaState()->DoString("Procedure_GameServer_SetState(\"conn_val_success\")");

	return PACKET_EXE_CONTINUE;
}
