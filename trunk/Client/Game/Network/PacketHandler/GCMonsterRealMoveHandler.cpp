#include "StdAfx.h"
#include "GCMonsterRealMove.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerNPC.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "GITimeSystem.h"
#include "GIUtil.h"

uint GCMonsterRealMoveHandler :: Execute( GCMonsterRealMove* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCMonsterRealMoveHandler");

	////��ǰ������������
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	//�������
	//	CObject_PlayerNPC* pNPC = pObjectManager->FindPlayerNPC( (INT)pPacket->getObjID() );
	//	if ( pNPC == NULL )
	//	{
	//		pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );
	//	}

	//	if(!pNPC) return PACKET_EXE_CONTINUE;

	//	fVector3 fvGame;
	//	CGameProcedure::s_pGfxSystem->Axis_Trans(
	//		tGfxSystem::AX_PLAN, fVector3(pPacket->getCurrentPos().m_fX, 0.0f, pPacket->getCurrentPos().m_fZ),
	//		tGfxSystem::AX_GAME, fvGame);

	//	SObject_PlayerNPCInit initPlayerNPC;
	//	// test code{
	//	switch(pPacket->getRace())
	//	{
	//	case 1:
	//		strcpy( initPlayerNPC.m_szActorFile, "General:ˮ��.obj" );
	//		initPlayerNPC.m_afSpeed[0] = 0.5f;	//walk
	//		initPlayerNPC.m_afSpeed[1] = 2.8f;	//run
	//		initPlayerNPC.m_afSpeed[2] = 2.0f;	//fly
	//		break;
	//
	//	case 2:
	//		strcpy( initPlayerNPC.m_szActorFile, "General:������.obj" );
	//		initPlayerNPC.m_afSpeed[0] = 0.5f;	//walk
	//		initPlayerNPC.m_afSpeed[1] = 2.8f;	//run
	//		initPlayerNPC.m_afSpeed[2] = 2.0f;	//fly
	//		break;
	//
	//	case 3:
	//		strcpy( initPlayerNPC.m_szActorFile, "General:ͭ��.obj" );
	//		initPlayerNPC.m_afSpeed[0] = 0.5f;	//walk
	//		initPlayerNPC.m_afSpeed[1] = 0.6f;	//run
	//		initPlayerNPC.m_afSpeed[2] = 2.0f;	//fly
	//		break;
	//
	//	case 0:
	//	default:
	//		strcpy( initPlayerNPC.m_szActorFile, "General:��.obj" );
	//		initPlayerNPC.m_afSpeed[0] = 0.5f;	//walk
	//		initPlayerNPC.m_afSpeed[1] = 0.4f;	//run
	//		initPlayerNPC.m_afSpeed[2] = 2.0f;	//fly
	//		break;
	//	}
	//	// }
	//	initPlayerNPC.m_fvPos			= fvGame;
	//	initPlayerNPC.m_dwMotionStatus	= CMS_STATUS_IDLE;

	//	pNPC->Initial( &initPlayerNPC );

	//	//�����µ�λ��
	//	pNPC->RegisterToZone();

	//	////�������
	//	//CObject_PlayerNPC* pNPC = pObjectManager->CreateNPC(
	//	//									(INT)pPacket->getObjID(),
	//	//									(INT)pPacket->getRace(),
	//	//									fVector2(pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ));

	//	//if(!pNPC) return PACKET_EXE_CONTINUE;

	//	//--- for debug

	//	////�����Ϣ�Ϸ���
	//	//fVector2 fvPosNow	( pNPC->GetPosition().x, pNPC->GetPosition().z);
	//	//fVector2 fvPosPacket( pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ);
	//	//FLOAT fDis = KLU_GetDist(fvPosNow, fvPosPacket);
	//	//if(fDis >= 5.0f)
	//	//{
	//	//	char szInvalidMsg[MAX_PATH];
	//	//	_snprintf(szInvalidMsg, MAX_PATH, 
	//	//		"InvlaidMove(%.1f,%.1f)\n(%.1f,%.1f)->(%.1f,%.1f)", 
	//	//		pNPC->GetPosition().x, pNPC->GetPosition().z,
	//	//		pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ,
	//	//		pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ);
	//	//	CGameProcedure::s_pGfxSystem->PushDebugString(szInvalidMsg);
	//	//	pNPC->PushDebugString(szInvalidMsg);
	//	//}
	//	//--- for debug


	//	//���õ�ǰ���Ŀ���
	//	//pNPC->SetCurAndTarget(
	//	//	fVector2(pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ),
	//	//	fVector2(pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ));
	//	SCommand_Object cmdTemp;
	//	cmdTemp.m_wID			= OC_MOVE_TO_PATH_NODE;
	//	cmdTemp.m_afParam[0]	= pPacket->getCurrentPos().m_fX;
	//	cmdTemp.m_afParam[1]	= pPacket->getCurrentPos().m_fZ;
	//	cmdTemp.m_afParam[2]	= pPacket->getTargetPos().m_fX;
	//	cmdTemp.m_afParam[3]	= pPacket->getTargetPos().m_fZ;

	//	pNPC->PushCommand( FALSE, &cmdTemp );

	//	char szTemp[MAX_PATH];
	//	_snprintf(szTemp, MAX_PATH, "RealMove(%.1f,%.1f)->(%.1f,%.1f)", 
	//		pPacket->getCurrentPos().m_fX, pPacket->getCurrentPos().m_fZ,
	//		pPacket->getTargetPos().m_fX, pPacket->getTargetPos().m_fZ);

	//	pNPC->PushDebugString(szTemp);
	//	pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

