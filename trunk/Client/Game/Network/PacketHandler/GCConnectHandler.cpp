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
		//����Ҫ����ĳ���������

		//!!Import 
		//ֻ�е�һ������ĳ������������Ϣ�������ĳ���id�ͽ���λ�ò�������
		//����������ӱ�ĳ����л��������ӱ�ķ������л�����������Ϣ
		//��������������ֵ�ǲ����ŵģ���Ҫʹ��GCNotifyChangeScene֪ͨ��ֵ

		BOOL bHave = FALSE;
		//����id
		CGameProcedure::s_pVariableSystem->GetAs_Int("Scene_ID", &bHave);
		if(!bHave)
		{
			CGameProcedure::s_pVariableSystem->SetAs_Int("Scene_ID", 
				(INT)pPacket->GetSceneID());
		}

		//����λ��
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
