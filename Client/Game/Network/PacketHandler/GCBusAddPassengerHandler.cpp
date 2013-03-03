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

#include "GCBusAddPassenger.h"

uint GCBusAddPassengerHandler::Execute( GCBusAddPassenger* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
		{
			return PACKET_EXE_CONTINUE;
		}

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_BUS_ADD_PASSENGER;
		cmdTemp.m_anParam[0]	= pPacket->GetIndex();
		cmdTemp.m_anParam[1]	= pPacket->GetPassengerID();
		pObj->PushCommand(&cmdTemp );

		pObj->PushDebugString("GCBusAddPassenger");
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

