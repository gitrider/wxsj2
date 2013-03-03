#include "StdAfx.h"

#include "..\..\NetWork\NetManager.h"

#include "GITimeSystem.h"
#include "GIException.h"
#include "CGCharAskBaseAttrib.h"

#include "..\..\GameCommand.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerNPC.h"
#include "Object\ObjectCommandDef.h"
#include "..\..\World\WorldManager.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"

#include "GCNewPet_Move.h"

uint GCNewPet_MoveHandler::Execute( GCNewPet_Move* pPacket, Player* pPlayer ) 
{
	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
//		CObjectManager* pObjectManager = CObjectManager::GetMe();
//
//		//���λ���Ƿ�Ϸ�
//		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
//		{
//			KLThrow("ERROR POSITION @ GCNewMonster_MoveHandler");
//		}
//		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ)))
//		{
//			KLThrow("ERROR POSITION @ GCNewMonster_MoveHandler target");
//		}
//
//		//�������
//		CObject_PlayerNPC* pNPC = (CObject_PlayerNPC*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
//		fVector3 fvGame;
//		CGameProcedure::s_pGfxSystem->Axis_Trans(
//			tGfxSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
//			tGfxSystem::AX_GAME, fvGame);
//
//		if ( pNPC == NULL )
//		{
//			pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );
//
//			SObjectInit initPlayerNPC;
//			initPlayerNPC.m_fvPos	= fvGame;
//			initPlayerNPC.m_fvRot	= fVector3( 0.f, 0.f, 0.f );
//
//			pNPC->Initial( &initPlayerNPC );
//		}
//		else
//		{
//			fVector2 mapPos(fvGame.x, fvGame.z);
//			pNPC->SetMapPosition(mapPos);
//			pNPC->Enable( OSF_VISIABLE );
//			pNPC->Disalbe( OSF_OUT_VISUAL_FIELD );
//		}
//		pNPC->SetNpcType(NPC_TYPE_PET);
//
//		pNPC->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());
//
//		// move to command
//		{
//			WORLD_POS posTarget;
//			posTarget.m_fX	= pPacket->getTargetPos().m_fX;
//			posTarget.m_fZ	= pPacket->getTargetPos().m_fZ;
//
//			SCommand_Object cmdTemp;
//			cmdTemp.m_wID			= OC_MOVE;
//			cmdTemp.m_auParam[0]	= 0;
//			cmdTemp.m_anParam[1]	= pPacket->getHandleID();
//			cmdTemp.m_anParam[2]	= 1;
//			cmdTemp.m_apParam[3]	= &posTarget;
//			pNPC->PushCommand(&cmdTemp );
//		}
//
//		//����Ask����
//		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pNPC->GetID());
//
//		// tempcode{
//		// �˰治����������æ�ͻ����Ӻ���Ϣ�Ĵ���
////		CGCharAskBaseAttrib msgAskBaseAttrib;
////		msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
////		CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );
//
//		// }
//
//
//		//ͬ����Ⱦ��
//		char szTemp[MAX_PATH];
//		_snprintf(szTemp, MAX_PATH, "GCNewPet_Move(%.1f,%.1f)", 
//			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
//		pNPC->PushDebugString(szTemp);
//		pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;
}

