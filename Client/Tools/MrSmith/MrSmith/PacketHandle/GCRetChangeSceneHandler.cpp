#include "StdAfx.h"
#include "GCRetChangeScene.h"

#include "../SMAgentManager.h"
#include "../SMAgent.h"
#include "../SMVariable.h"
#include "../SMWorldManager.h"
#include "../SMProcedureManager.h"
#include "LuaPlus.h"

using namespace Packets;
UINT GCRetChangeSceneHandler::Execute(GCRetChangeScene* pPacket, Player* pPlayer)
{
	SMITH::Agent*				pAgent	= (SMITH::Agent*)pPlayer;
	std::pair< float, float >	fvPos	= pAgent->getVariable()->getAs_Vector2("DestScene_EnterPos");

	if( pPacket ){
		if(pPacket->GetReturn() == GCRetChangeScene::CSR_ERROR)
		{
			//不允许进入
			pAgent->getLuaState()->DoString("Procedure_EnterScene_SetState(\"change_err\")");
		}
		else if(pPacket->GetReturn() == GCRetChangeScene::CSR_SUCCESS)
		{
			pAgent->getVariable()->setAs_Int("Scene_ID", pAgent->getVariable()->getAs_Int("DestScene_ID") );
			pAgent->getVariable()->setAs_Vector2("Scene_EnterPos", fvPos.first, fvPos.second );

			pAgent->getProcedureManager()->SetCurrentProcedure( "EnterScene" );
			pAgent->getLuaState()->DoString("Procedure_EnterScene_SetState(\"change_ok\")");
		}
		else if(pPacket->GetReturn() == GCRetChangeScene::CSR_SUCCESS_DIFFSERVER)
		{
			pAgent->getVariable()->setAs_Int("Scene_ID", pAgent->getVariable()->getAs_Int("DestScene_ID") );
			pAgent->getVariable()->setAs_Vector2("Scene_EnterPos", fvPos.first, fvPos.second );
			pAgent->getVariable()->setVariable( "GameServer_Address", pPacket->GetIP() );
			pAgent->getVariable()->setAs_Int( "Game_Server_Port", pPacket->GetPort() );
			pAgent->getVariable()->setAs_Int( "GameServer_Key", pPacket->GetKey() );
			
			pAgent->getProcedureManager()->SetCurrentProcedure( "EnterScene" );
			pAgent->getLuaState()->DoString("Procedure_EnterScene_SetState(\"change_ok2\")");
		}
	}

	return PACKET_EXE_CONTINUE;
}
