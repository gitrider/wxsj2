#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewPlayer_Death.h"
#include "..\..\World\WorldManager.h"
#include "GIException.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"

uint GCNewPlayer_DeathHandler :: Execute( GCNewPlayer_Death* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewPlayerHandler");
//	return PACKET_EXE_CONTINUE ;

	//��ǰ������������
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		//���λ���Ƿ�Ϸ�
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCNewPlayer_DeathHandler");
		}

		//�������
		CObject_PlayerOther* pOther = (CObject_PlayerOther*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			tGfxSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
			tGfxSystem::AX_GAME, fvGame);

		FLOAT fFaceDir = pPacket->getDir();

		if ( pOther == NULL )
		{
			pOther = pObjectManager->NewPlayerOther( (INT)pPacket->getObjID() );

			SObjectInit initPlayerOther;
			initPlayerOther.m_fvPos	= fvGame;
			initPlayerOther.m_fvRot = fVector3( 0.f, fFaceDir, 0.f );

			pOther->Initial( &initPlayerOther );
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pOther->SetMapPosition(mapPos);
			pOther->SetFaceDir( fFaceDir );
			pOther->Enable( OSF_VISIABLE );
			pOther->Disalbe( OSF_OUT_VISUAL_FIELD );
		}

		//����װ����Ϣ
		pOther->GetCharacterData()->Set_EquipVer(pPacket->getEquipVer());

		pOther->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_DEATH;
		cmdTemp.m_abParam[0]	= FALSE;
		pOther->PushCommand(&cmdTemp );

		//����Ask����
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pOther->GetID());

		// tempcode{
		// �˰治����������æ�ͻ����Ӻ���Ϣ�Ĵ���
//		CGCharAskBaseAttrib msgAskBaseAttrib;
//		msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
//		CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		//AxTrace(0, 0, "[%d]Ask %d", m_hmObject.size(), pPacket->getObjID());
		// }

		pOther->PushDebugString("GCNewPlayer");
		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
//__ENTER_FUNCTION
//	AxTrace(0, 2, "GCNewPlayerHandler");
//
//	//��ǰ������������
//	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
//	{
//		CObjectManager* pObjectManager = CObjectManager::GetMe();
//
//		//�������
//		CObject_PlayerOther* pOther = pObjectManager->CreatePlayerOther(
//											(INT)pPacket->getObjID(),
//											(INT)pPacket->getRace(),
//											pPacket->getEquipVer(),
//											fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ),
//											true);
//
//		if(!pOther) return PACKET_EXE_CONTINUE;
//
//		//�����µ�λ��
//		pOther->SetMapPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
//		pOther->RegisterToZone();
//		//IDLEװ̬
//		pOther->SetMotionStatus(CMS_STATUS_IDLE);
//		pOther->PushDebugString("GCNewPlayer");
//		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
//	}
//
//	return PACKET_EXE_CONTINUE ;
//
//__LEAVE_FUNCTION
//
//	return PACKET_EXE_ERROR ;
}

