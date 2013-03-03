#include "StdAfx.h"
#include "GCDie.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "Object\Logic\Character\AI\GMAICommandDef.h"
#include "Object\Logic\Character\AI\GMAI_Base.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "GITimeSystem.h"
#include "..\..\Global.h"
#include "GIDEbuger.h"
#include "..\..\Object\GMObjectLoadQueue.h"

uint GCDieHandler::Execute( GCDie* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//ASSERT(FALSE && "GCDieHandler::Execute �����Ϣ�Ѿ���ʱ");
	////return PACKET_EXE_CONTINUE ;
	//CGameProcedure::s_pGfxSystem->PushDebugString("GCDieHandler");

	////��ǰ������������
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	//Ѱ��������
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	CObject_Character* pChar = (CObject_Character*)pObjectManager->FindServerObject((INT)pPacket->getObjID());
	//	if(!pChar) return PACKET_EXE_CONTINUE;

	//	//�����Ҫ������Դ�����Է��뻺�����
	//	if(pObjectManager->GetLoadQueue()->TryAddLoadTask(pChar->GetID(), pPacket))
	//		return PACKET_EXE_NOTREMOVE;

	//	//SCommand_Object cmdTemp;
	//	//cmdTemp.m_wID			= OC_DEATH;
	//	//cmdTemp.m_abParam[0]	= true;
	//	//pChar->PushCommand(&cmdTemp );

	//	_DAMAGE_INFO infoDamage;
	//	infoDamage.m_nSkillID				= 0;
	//	infoDamage.m_nType					= infoDamage.TYPE_DIE;
	//	infoDamage.m_nImpactID				= SPECIAL_DIE; 				

	//	SCommand_Object cmdTemp;
	//	cmdTemp.m_wID			= OC_LOGIC_EVENT;
	//	cmdTemp.m_apParam[0]	= (VOID*)(&infoDamage);
	//	pChar->PushCommand(&cmdTemp );

	//	//if ( g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_Character))
	//	//	&& pChar->CharacterLogic_GetAI() != NULL )
	//	//{
	//	//	SCommand_AI cmdAI;
	//	//	cmdAI.m_wID = AIC_DIE;
	//	//	pChar->CharacterLogic_GetAI()->PushCommand( &cmdAI );
	//	//}

	//	if(pChar == CObjectManager::GetMe()->GetMainTarget()
	//		|| pChar == (CObject_Character*)(CObjectManager::GetMe()->GetMySelf())
	//		)
	//	{
	//		CObjectManager::GetMe()->SetMainTarget(INVALID_ID);
	//	}

	//	pChar->PushDebugString("GCDie");
	//	pChar->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}