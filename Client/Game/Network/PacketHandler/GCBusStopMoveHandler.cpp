#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\..\Network\NetManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\Manager\ObjectManager.h"
#include "..\..\Object\Logic\Object.h"
#include "..\..\Object\Logic\Bus\Obj_Bus.h"
#include "..\..\Object\ObjectDef.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\World\WorldManager.h"
#include "GIException.h"

#include "GCBusStopMove.h"

uint GCBusStopMoveHandler::Execute( GCBusStopMove* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//检查位置是否合法
		const WORLD_POS* pTargetPos = pPacket->GetCurPos();
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pTargetPos->m_fX, pTargetPos->m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCBusStopMoveHandler target");
		}

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
		{
			return PACKET_EXE_CONTINUE;
		}

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_BUS_STOP_MOVE;
		cmdTemp.m_afParam[0]	= pPacket->GetCurPos()->m_fX;
		cmdTemp.m_afParam[1]	= -1.f;
		cmdTemp.m_afParam[2]	= pPacket->GetCurPos()->m_fZ;
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCBusStopMove");
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

