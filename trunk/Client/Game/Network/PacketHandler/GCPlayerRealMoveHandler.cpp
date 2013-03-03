#include "StdAfx.h"
#include "GCPlayerRealMove.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerOther.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GITimeSystem.h"

uint GCPlayerRealMoveHandler :: Execute( GCPlayerRealMove* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCPlayerRealMoveHandler[%f,%f]->[%f,%f]", 
	//	pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ, 
	//	pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ);

	////��ǰ������������
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	//�������
	//	CObject_PlayerOther* pOther = pObjectManager->FindPlayerOther( (INT)pPacket->getObjID() );
	//	if ( pOther == NULL )
	//	{
	//		pOther = pObjectManager->NewPlayerOther( (INT)pPacket->getObjID() );
	//	}

	//	if(!pOther) return PACKET_EXE_CONTINUE;

	//	//----------------------------------------------
	//	//�����Ҫ����װ����Ϣ
	//	UINT dwEquipVer = pPacket->getEquipVer();
	//	if ( pOther->GetEquipVer() < dwEquipVer )
	//	{
	//		//����װ��������Ϣ
	//		pOther->SendEuipRequest();
	//		pOther->SetEquipVer(dwEquipVer);
	//	}

	//	fVector3 fvGame;
	//	CGameProcedure::s_pGfxSystem->Axis_Trans(
	//		tGfxSystem::AX_PLAN, fVector3(pPacket->getCurrentPos().m_fX, 0.0f, pPacket->getCurrentPos().m_fZ),
	//		tGfxSystem::AX_GAME, fvGame);

	//	SObject_PlayerOtherInit initPlayerOther;
	//	strcpy( initPlayerOther.m_szActorFile, "General:Ů����.obj" );
	//	initPlayerOther.m_afSpeed[0]		= 0.5f;	//walk
	//	initPlayerOther.m_afSpeed[1]		= 2.8f;	//run
	//	initPlayerOther.m_afSpeed[2]		= 2.0f;	//fly
	//	initPlayerOther.m_fvPos				= fvGame;
	//	initPlayerOther.m_dwMotionStatus	= CMS_STATUS_IDLE;

	//	pOther->Initial( &initPlayerOther );

	//	//�����µ�λ��
	//	pOther->RegisterToZone();
	//	////�������
	//	//CObject_PlayerOther* pOther = pObjectManager->CreatePlayerOther(
	//	//									(INT)pPacket->getObjID(),
	//	//									(INT)pPacket->getRace(),
	//	//									pPacket->getEquipVer(),
	//	//									fVector2(pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ),
	//	//									true);

	//	//if(!pOther) return PACKET_EXE_CONTINUE;

	//	//���õ�ǰ���Ŀ���
	//	//pOther->SetCurAndTarget(
	//	//	fVector2(pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ),
	//	//	fVector2(pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ));
	//	SCommand_Object cmdTemp;
	//	cmdTemp.m_wID			= OC_MOVE_TO_PATH_NODE;
	//	cmdTemp.m_afParam[0]	= pPacket->getCurrentPos().m_fX;
	//	cmdTemp.m_afParam[1]	= pPacket->getCurrentPos().m_fZ;
	//	cmdTemp.m_afParam[2]	= pPacket->getTargetPos().m_fX;
	//	cmdTemp.m_afParam[3]	= pPacket->getTargetPos().m_fZ;

	//	pOther->PushCommand( FALSE, &cmdTemp );

	//	pOther->PushDebugString("GCPlayerRealMove");
	//	pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

