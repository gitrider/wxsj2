#include "stdafx.h"
#include "GCConnect.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Procedure\GamePro_Login.h"
#include "..\..\Procedure\Gamepro_ChangeScene.h"
#include "GIVariable.h"
#include "..\..\World\WorldManager.h"

uint GCConnectHandler::Execute( GCConnect* pPacket, Player* pPlayer )
{
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcChangeScene)
	{
		//设置要进入的场景的数据

		//!!Import 
		//只有第一次连接某服务器，该消息发回来的场景id和进入位置才有意义
		//其他情况（从别的场景切换过来，从别的服务器切换过来）该消息
		//发回来的这两个值是不可信的，需要使用GCNotifyChangeScene通知的值

		BOOL bHave = FALSE;
		//场景id
		CGameProcedure::s_pVariableSystem->GetAs_Int("Scene_ID", &bHave);
		if(!bHave)
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("Scene_ID", 
				(INT)pPacket->GetSceneID());
		}

		//场景位置
		CGameProcedure::s_pVariableSystem->GetAs_Int("Scene_EnterPos", &bHave);
		if(!bHave)
		{
			CGameProcedure::s_pVariableSystem->SetAs_Vector2("Scene_EnterPos", (FLOAT)pPacket->GetWorldPos()->m_fX, (FLOAT)pPacket->GetWorldPos()->m_fZ);
		}

		if( CGameProcedure::s_pProcChangeScene )
		{
			CGameProcedure::s_pProcChangeScene->SetStatus(CGamePro_ChangeScene::CHANGESCENE_RECEIVE_CGCONNECT_SUCCESS);
		}
	}

	return PACKET_EXE_CONTINUE ;
}
