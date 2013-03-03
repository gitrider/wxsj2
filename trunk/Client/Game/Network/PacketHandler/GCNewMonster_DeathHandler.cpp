
#include "StdAfx.h"
#include "GITimeSystem.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerNPC.h"
#include "GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "Interface\GMGameInterface.h"
#include "NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewMonster_Death.h"
#include "World\WorldManager.h"
#include "GIException.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Object\GMObjectLoadQueue.h"



uint GCNewMonster_DeathHandler :: Execute( GCNewMonster_Death* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION

	// 当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		// 检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewMonster_DeathHandler");
		}

		// 创建玩家
		CObject_PlayerNPC* pNPC = (CObject_PlayerNPC*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(tGfxSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
												 tGfxSystem::AX_GAME, fvGame);

		FLOAT fFaceDir = pPacket->getDir();
		if( pNPC == NULL )
		{
			pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );
			//AxTrace(0, 0, "New Monster_Death");

			SObjectInit initPlayerNPC;

			initPlayerNPC.m_fvPos = fvGame;
			initPlayerNPC.m_fvRot = fVector3( 0.f, fFaceDir, 0.f );

			pNPC->Initial( &initPlayerNPC );
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pNPC->SetMapPosition(mapPos);
			pNPC->SetFaceDir( fFaceDir );
			pNPC->Enable( OSF_VISIABLE );
			pNPC->Disalbe( OSF_OUT_VISUAL_FIELD );
		}

		pNPC->SetNpcType(NPC_TYPE_MONSTER);

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_DEATH;
		cmdTemp.m_abParam[0]	= FALSE;
		pNPC->PushCommand(&cmdTemp );

		// 清除界面选中NPC
		if(CObjectManager::GetMe()->GetMainTarget() && 
			pNPC &&
			((CObject*)(CObjectManager::GetMe()->GetMainTarget()))->GetID() == pNPC->GetID())
		{
			CGameInterface::GetMe()->Object_SelectAsMainTarget(INVALID_ID, 0);
		}

		pNPC->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());	// 移动速度
		pNPC->GetCharacterData()->Set_MountID( -1 );						// 坐骑
		pNPC->GetCharacterData()->Set_MonstWeapon( -1 );					// 武器


		// 放入Ask队列
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pNPC->GetID());
		// tempcode{
		// 此版不做服务器繁忙客户端延后发消息的处理
//		CGCharAskBaseAttrib msgAskBaseAttrib;
//		msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
//		CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		//AxTrace(0, 0, "[%d]Ask %d", m_hmObject.size(), pPacket->getObjID());
		// }

		//同步渲染层
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "GCNewMonster_Death(%.1f,%.1f)", 
			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
		pNPC->PushDebugString(szTemp);
		pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
		//AxTrace(0, 2, "********NewMonster_Death End");
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

