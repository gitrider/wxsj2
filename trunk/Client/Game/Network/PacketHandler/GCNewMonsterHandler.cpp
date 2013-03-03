
#include "StdAfx.h"
#include "GITimeSystem.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerNPC.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "Object\GMObjectLoadQueue.h"
#include "NetWork\NetManager.h"
#include "Object\ObjectCommandDef.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewMonster.h"
#include "World\WorldManager.h"
#include "GIException.h"
#include "AxProfile.h"
#include "GIVariable.h"
#include "DataPool\GMDP_CharacterData.h"
#include "game\GIGameInterface.h"
#include "Object\Logic\Character\Obj_Character.h"

uint GCNewMonsterHandler :: Execute( GCNewMonster* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION

	// ��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		// ���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewMonsterHandler");
		}

		// ����Npc
		CObject_PlayerNPC* pNPC = (CObject_PlayerNPC*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans
			( tGfxSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ), tGfxSystem::AX_GAME, fvGame );


		// ����λ�úͷ���

		// ����
		FLOAT fFaceDir = pPacket->getDir();

		// �����ڣ� �����µ�
		if( pNPC == NULL )
		{
			pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );

			SObjectInit initPlayerNPC;

			initPlayerNPC.m_fvPos = fvGame;
			initPlayerNPC.m_fvRot = fVector3( 0.f, fFaceDir, 0.f );

			pNPC->Initial( &initPlayerNPC );

			// 20100514 AddCodeBegin �����зŵ��˴������NPC���½������ģ���������ϸ����
			// ����Ask����
			CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask( pNPC->GetID() );
			// 20100514 AddCodeEnd

		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pNPC->SetMapPosition(mapPos);
			pNPC->SetFaceDir( fFaceDir );
			pNPC->Enable( OSF_VISIABLE );
			pNPC->Disalbe( OSF_OUT_VISUAL_FIELD );
		}

		// ��ʶΪ��
		pNPC->SetNpcType( NPC_TYPE_MONSTER );
		// �����ƶ��ٶ�
		pNPC->GetCharacterData()->Set_MoveSpeed( pPacket->getMoveSpeed() );
		// ��������
		pNPC->GetCharacterData()->Set_MountID( pPacket->getHorseID() );
		// ��������װ��
		pNPC->GetCharacterData()->Set_MonstWeapon( pPacket->getWeaponID() );


		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_IDLE;
		cmdTemp.m_afParam[0]	= fvGame.x;
		cmdTemp.m_afParam[1]	= fvGame.z;
		cmdTemp.m_abParam[2]	= FALSE;

		pNPC->PushCommand( &cmdTemp );


		// 20100514 DeleteCodeBegin
		// ����Ask����
		//CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask( pNPC->GetID() );
		// 20100514 DeleteCodeEnd


		// ͬ����Ⱦ��
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "GCNewMonster(%.1f,%.1f)", pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
		pNPC->PushDebugString(szTemp);
		pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());



		// 20100415 AddCodeBegin
		// �Զ�Ѱ·��NPC���ݵ������������������NPC�Ի���
		if ( CObjectManager::GetMe()->GetMySelf()->GetIsAutoNPCPath()->m_isAutoPath == TRUE &&
			 CObjectManager::GetMe()->GetMySelf()->GetIsAutoNPCPath()->m_strAutoNPCName == pNPC->GetCharacterData()->Get_Name() &&
			CGameProcedure::s_pWorldManager->GetActiveSceneID() == CObjectManager::GetMe()->GetMySelf()->GetIsAutoNPCPath()->m_nSceneID )
		{
			CObject_Character* pCharObj  = NULL;
			STRING			   strName   = CObjectManager::GetMe()->GetMySelf()->GetIsAutoNPCPath()->m_strAutoNPCName;
			pCharObj  = CObjectManager::GetMe()->FindCharacterByName( strName );

			if (  pCharObj != NULL )
			{				
				int i = pCharObj->GetServerID();
				CGameProcedure::s_pGameInterface->Player_Speak( i );

				if ( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_IsStopped( CObject_Character::LOGIC_BASE ) )
				{
					CObjectManager::GetMe()->GetMySelf()->SetIsAutoNPCPath(-1, "", FALSE );
				}
			}	
	
		}
		// 20100415 AddCodeEnd

	}

	return PACKET_EXE_CONTINUE ;
}

