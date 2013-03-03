
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
			// 等待服务器的回应
			SetStatus(ENTERSCENE_READY);
			break;
		}
	// 发送进入场景的请求
	case ENTERSCENE_READY:
		{
			INT nSceneID = s_pVariableSystem->GetAs_Int("Scene_ID");

			// 发送进入场景的请求
			CGEnterScene msg;
			msg.setEnterType((BYTE)m_dwEnterType);
			msg.setSceneID(nSceneID);
			fVector2 fvEnterPos = s_pVariableSystem->GetAs_Vector2("Scene_EnterPos");
			msg.setEnterPos(WORLD_POS(fvEnterPos.x, fvEnterPos.y));
			
			CNetManager::GetMe()->SendPacket(&msg);

			// 等待服务器的回应
			SetStatus(ENTERSCENE_REQUESTING);
		}
		break;

	// 发送进入场景的请求,等待服务器的回应...
	case ENTERSCENE_REQUESTING:
		break;

	// 服务器允许进入场景
	case ENTERSCENE_OK:
		{
			// 关闭系统界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
			// 关闭帐号输入界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
			// 关闭人物选择界面
			CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
			// 设置场景摄像机
			CGameProcedure::s_pGfxSystem->Camera_SetCurrent(tGfxSystem::SCENE);
			
			EnterScene();
		}
		break;

	// 服务器不允许进入场景
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
	// 进入场景
	// 如果当前有已经Load得场景，首先会退出并销毁场景上所有的Object，包括自身

	//清除人物相机透明物体列表
	Fairy::System* pFairySystem = CEngineInterface::GetMe()->GetFairySystem();
	pFairySystem->clearTransparencyObjects();

	//616======* 单机 设置场景
	if(CGameProcedure::s_pVariableSystem->GetAs_Int("Game_EnterDirect") == 1)
	{
		m_nSceneID = 1;
	}


	if(!(s_pWorldManager->EnterScene(m_nSceneID, m_nCityLevel, m_nResID)))
	{
		return;
	}

	// 删除界面模型
	CObjectManager::GetMe()->DestoryActorAvatar();

	//------------------------------------------------------------------
	//创建玩家自身
	INT idMySelfServer = (INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("MySelf_ID"));
	CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
	if(!pMySelf)
	{
		pMySelf = (CObject_PlayerMySelf*)CObjectManager::GetMe()->NewObject(_T("CObject_PlayerMySelf"), idMySelfServer);
	}

	//------------------------------------------------------------------
	//设置玩家自身数据

	//设置ServerID
	CObjectManager::GetMe()->SetObjectServerID( pMySelf->GetID(), idMySelfServer );

	//设置 GUID
	pMySelf->SetServerGUID((INT)(CGameProcedure::s_pVariableSystem->GetAs_Int("User_GUID")));

	//设置初始位置
	fVector2 fvMapPos = CGameProcedure::s_pVariableSystem->GetAs_Vector2("MySelf_Pos");

	//616======* 设置位置
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

	//重新设置位置，防止进入行走面下面
	CObjectManager::GetMe()->GetMySelf()->SetMapPosition(fvMapPos);

	//同步摄像机
	CGameProcedure::s_pGfxSystem->Camera_SetLookAt(CObjectManager::GetMe()->GetMySelf()->GetPosition());

	//产生事件
	s_pEventSystem->PushEvent(GE_PLAYER_ENTER_WORLD);

	// 人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);

	// 背景界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_BACK_GROUND);



	// 关闭系统界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SYSTEM_INFO);
	// 关闭帐号输入界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_COUNT_INPUT);
	// 关闭人物选择界面
	CGameProcedure::s_pEventSystem->PushEvent( GE_GAMELOGIN_CLOSE_SELECT_CHARACTOR);
	//设置场景摄像机
	CGameProcedure::s_pGfxSystem->Camera_SetCurrent(tGfxSystem::SCENE);
	
	// 设置摄象机旋转角度
	DBC_DEFINEHANDLE(s_pSceneDBC, DBC_SCENE_DEFINE);
	const _DBC_SCENE_DEFINE* pTempSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_LineNum_EQU( m_nSceneID );
	if( pTempSceneDef )
	{
		CGameProcedure::s_pGfxSystem->Camera_AddDirection( pTempSceneDef->nCameraAngle );
	}

	// 设置摄像机位置偏移
	BOOL bHave = FALSE;
	INT fCamera_Offset = CGameProcedure::s_pVariableSystem->GetAs_Float( "Camera_Varible_Offset", &bHave );
	if( bHave )
		CGameProcedure::s_pGfxSystem->Camera_SetOffset( (tGfxSystem::OFFSET_TAG)0, 0, fCamera_Offset );



	//转入主循环
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
