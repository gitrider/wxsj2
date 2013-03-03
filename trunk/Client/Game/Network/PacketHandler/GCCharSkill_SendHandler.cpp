
#include "StdAfx.h"
#include "GITimeSystem.h"
#include "Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "Object\ObjectCommandDef.h"
#include "GameCommand.h"
#include "GCCharSkill_Send.h"

#include "Object\Logic\Character\Ai\GMAI_Base.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "Object\ObjectCommand.h"


using namespace Packets;

uint GCCharSkill_SendHandler::Execute( GCCharSkill_Send* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION


//	AxTrace(0, 2, "GCCharSkill_SendHandler::Execute");
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		//CObjectManager* pObjectManager = CObjectManager::GetMe();

		//CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		//if ( pObj == NULL )
		//	return PACKET_EXE_CONTINUE;

		//SCommand_Object cmdTemp;

		//ObjID_t	dwTargetIDList[MAGICSEND_MAX_TARGET_NUM] = {0};

		//dwTargetIDList[0] = pPacket->getTargetID();
		//
		//cmdTemp.m_wID			= OC_MAGIC_SEND;
		//cmdTemp.m_auParam[0]	= 0;
		//cmdTemp.m_anParam[1]	= pPacket->getLogicCount();
		//cmdTemp.m_anParam[2]	= pPacket->getSkillDataID();
		//cmdTemp.m_anParam[3]	= 1;
		//cmdTemp.m_afParam[4]	= pPacket->getTargetPos().m_fX;
		//cmdTemp.m_afParam[5]	= pPacket->getTargetPos().m_fZ;
		//cmdTemp.m_afParam[6]	= pPacket->getDir();
		//cmdTemp.m_apParam[7]	= (VOID*)&dwTargetIDList;

		//pObj->PushCommand(&cmdTemp );

		//pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

