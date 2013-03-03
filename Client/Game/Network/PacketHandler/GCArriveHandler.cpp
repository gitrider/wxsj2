#include "StdAfx.h"
#include "GCArrive.h"
#include "GITimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Object.h"
#include "..\..\GameCommand.h"
#include "Object\ObjectCommandDef.h"
#include "Object\Logic\Character\AI\GMAICommandDef.h"
#include "Object\Logic\Character\AI\GMAI_Base.h"
#include "Object\Logic\Character\Obj_Character.h"
#include "..\..\Global.h"

#include "..\..\World\WorldManager.h"
#include "GIException.h"

uint GCArriveHandler :: Execute( GCArrive* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			KLThrow("ERROR POSITION @ GCArriveHandler");
		}

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_STOP_MOVE;
		cmdTemp.m_anParam[0]	= pPacket->getHandleID();
		cmdTemp.m_anParam[1]	= 0;
		cmdTemp.m_afParam[2]	= pPacket->getWorldPos().m_fX;
		cmdTemp.m_afParam[3]	= pPacket->getWorldPos().m_fZ;
		pObj->PushCommand(&cmdTemp );

		CHAR szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH ,"GCArrive[%.1f,%.1f]", 
			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);

		pObj->PushDebugString(szTemp);
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}
	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

