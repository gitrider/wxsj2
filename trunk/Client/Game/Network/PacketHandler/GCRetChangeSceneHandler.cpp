
#include "StdAfx.h"
#include "GCRetChangeScene.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Procedure\GamePro_Enter.h"
#include "..\..\Procedure\GamePro_Main.h"
#include "..\..\World\WorldManager.h"
#include "GIVariable.h"
#include "CGEnterScene.h"
#include "..\..\Global.h"
#include "..\..\Event\GMEventSystem.h"


uint GCRetChangeSceneHandler::Execute( GCRetChangeScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	//AxTrace(0, 0, "*********GCRetChangeSceneHandler");
	
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain &&
		CWorldManager::GetMe()->GetStation() == tWorldSystem::WS_ASK_CHANGE_SCANE)
	{
		//���������
		if(pPacket->GetReturn() == GCRetChangeScene::CSR_ERROR)
		{
			CGameProcedure::s_pGfxSystem->PushDebugString("ChangeScene Denied");
			return PACKET_EXE_CONTINUE;
		}

		// ����Ҫ�л�������
		if( pPacket->GetReturn() == GCRetChangeScene::CSR_SUCCESS )
		{
			// ���ü�������ĳ���������
			CGameProcedure::s_pVariableSystem->SetAs_Int("Scene_ID", CWorldManager::GetMe()->GetNextSenceID()),
			CGameProcedure::s_pVariableSystem->SetAs_Vector2("Scene_EnterPos", 
				CWorldManager::GetMe()->GetNextScenePos().x,
				CWorldManager::GetMe()->GetNextScenePos().y	),

			CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SCENE_TRANS);
			return PACKET_EXE_CONTINUE;
		}

		// ��Ҫ�л�������
		if( pPacket->GetReturn() == GCRetChangeScene::CSR_SUCCESS_DIFFSERVER )
		{
			//���ü�������ĳ���������
			CGameProcedure::s_pVariableSystem->SetVariable("GameServer_Address", pPacket->GetIP()),
			CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Port", pPacket->GetPort()),
			CGameProcedure::s_pVariableSystem->SetAs_Int("GameServer_Key", pPacket->GetKey()),
			CGameProcedure::s_pVariableSystem->SetAs_Int("Scene_ID", CWorldManager::GetMe()->GetNextSenceID()),
			CGameProcedure::s_pVariableSystem->SetAs_Vector2("Scene_EnterPos", 
				CWorldManager::GetMe()->GetNextScenePos().x,
				CWorldManager::GetMe()->GetNextScenePos().y	),

			CGameProcedure::s_pEventSystem->PushEvent(GE_ON_SERVER_TRANS);
			return PACKET_EXE_CONTINUE;
		}
	}


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
