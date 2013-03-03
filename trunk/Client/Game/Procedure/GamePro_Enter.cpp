
#include "StdAfx.h"
#include "GamePro_Enter.h"
#include "Network\NetManager.h"
#include "DataPool\GMUIDataPool.h"
#include "DataPool\DataPoolCommandDef.h"
#include "GameCommand.h"
#include "World\WorldManager.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "CGConnect.h"
#include "CGEnterScene.h"
#include "Global.h"
#include "GIVariable.h"
#include "GIEventDefine.h"
#include "GamePro_Login.h"
#include "DBC\GMDataBase.h"
#include "Core\FairySystem.h"
#include "Engine\EngineInterface.h"

CGamePro_Enter::CGamePro_Enter()
{
	m_Status = ENTERSCENE_READY;
	m_dwEnterType = ENTER_TYPE_FIRST;
}

CGamePro_Enter::~CGamePro_Enter()
{
}


VOID CGamePro_Enter::Init(VOID)
{
}

VOID CGamePro_Enter::Tick(VOID)
{
	CGameProcedure::Tick();

	switch(GetStatus())
	{
	case ENTERSCENE_CONNECTING:
		{
			// �ȴ��������Ļ�Ӧ
			SetStatus(ENTERSCENE_READY);
			break;
		}
	// ���ͽ��볡��������
	case ENTERSCENE_READY:
		{
			INT nSceneID = s_pVariableSystem->GetAs_Int("Scene_ID");

			// ���ͽ��볡��������
			CGEnterScene msg;
			msg.setEnterType((BYTE)m_dwEnterType);
			msg.setSceneID(nSceneID);
			fVector2 fvEnterPos = s_pVariableSystem->GetAs_Vector2("Scene_EnterPos");
			msg.setEnterPos(WORLD_POS(fvEnterPos.x, fvEnterPos.y));
			
			CNetManager::GetMe()->SendPacket(&msg);

			// �ȴ��������Ļ�Ӧ
			SetStatus(ENTERSCENE_REQUESTING);
		}
		break;

	// ���ͽ��볡��������,�ȴ��������Ļ�Ӧ...
	case ENTERSCENE_REQUESTING:
		break;

	// ������������볡��
	case ENTERSCENE_OK:
		{
			// �ر�ϵͳ����
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
			// �ر��ʺ��������
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
			// �ر�����ѡ�����
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
			// ���ó��������
			CGameProcedure::s_pGfxSystem->Camera_SetCurrent(tGfxSystem::SCENE);
			
			EnterScene();
		}
		break;

	// ��������������볡��
	case ENTERSCENE_FAILED:
		{
			//static BOOL bShowInfo = FALSE;

			//if(!bShowInfo)
			//{
			//	bShowInfo = TRUE;
			//	STRING strTemp = NOCOLORMSGFUNC("Login_enterscene_rejected");
			//	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_SHOW_SYSTEM_INFO_CLOSE_NET, strTemp.c_str());	
			//}//
		}
		break;
	}
}

VOID CGamePro_Enter::EnterScene(void)
{
	// ���볡��
	// �����ǰ���Ѿ�Load�ó��������Ȼ��˳������ٳ��������е�Object����������

	//����������͸�������б�
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	pFairySystem->clearTransparencyObjects();

	//616======* ���� ���ó���
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
	{
		m_nSceneID = 1;
	}


	if(!(s_pWorldManager->EnterScene(m_nSceneID, m_nCityLevel, m_nResID)))
	{
		return;
	}

	// ɾ������ģ��
	CObjectManager::GetMe()->DestoryActorAvatar();

	//------------------------------------------------------------------
	//�����������
	INT idMySelfServer = (INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("MySelf_ID"));
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(!pMySelf)
	{
		pMySelf = (CObject_PlayerMySelf*)CObjectManager::GetMe()->NewObject(_T("CObject_PlayerMySelf"), idMySelfServer);
	}

	//------------------------------------------------------------------
	//���������������

	//����ServerID
	CObjectManager::GetMe()->SetObjectServerID( pMySelf->GetID(), idMySelfServer );

	//���� GUID
	pMySelf->SetServerGUID((INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("User_GUID")));

	//���ó�ʼλ��
	fVector2 fvMapPos = CGameProcedure::s_pVariableSystem->GetAs_Vector2("MySelf_Pos");

	//616======* ����λ��
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
	{
		fvMapPos.x = 44;
		fvMapPos.y = 33;
	}


	fVector3 fvGame;
	CGameProcedure::s_pGfxSystem->Axis_Trans(
		tGfxSystem::AX_PLAN, fVector3(fvMapPos.x, 0.0f, fvMapPos.y),
		tGfxSystem::AX_GAME, fvGame);

	SObjectInit initPlayerMySelf;
	initPlayerMySelf.m_fvPos = fvGame;
	CObjectManager::GetMe()->GetMySelf()->Initial(&initPlayerMySelf);

	//��������λ�ã���ֹ��������������
	CObjectManager::GetMe()->GetMySelf()->SetMapPosition(fvMapPos);

	//ͬ�������
	CGameProcedure::s_pGfxSystem->Camera_SetLookAt(CObjectManager::GetMe()->GetMySelf()->GetPosition());

	//�����¼�
	s_pEventSystem->PushEvent(GE_PLAYER_ENTER_WORLD);

	// ����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_BACK_GROUND);



	// �ر�ϵͳ����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
	// �ر��ʺ��������
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
	// �ر�����ѡ�����
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
	//���ó��������
	CGameProcedure::s_pGfxSystem->Camera_SetCurrent(tGfxSystem::SCENE);
	
	// �����������ת�Ƕ�
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU( m_nSceneID );
	if( pTempSceneDef )
	{
		CGameProcedure::s_pGfxSystem->Camera_AddDirection( pTempSceneDef->nCameraAngle );
	}

	// ���������λ��ƫ��
	BOOL bHave = FALSE;
	INT fCamera_Offset = CGameProcedure::s_pVariableSystem->GetAs_Float( "Camera_Varible_Offset", &bHave );
	if( bHave )
		CGameProcedure::s_pGfxSystem->Camera_SetOffset( (tGfxSystem::OFFSET_TAG)0, 0, fCamera_Offset );



	//ת����ѭ��
	CGameProcedure::SetActiveProc((CGameProcedure*)CGameProcedure::s_pProcMain);

}

VOID CGamePro_Enter::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Enter::Release(VOID)
{

}

VOID CGamePro_Enter::MouseInput(VOID)
{

}
