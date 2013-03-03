#include "StdAfx.h"
#include "GCSouXiaDiscard.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"

using namespace Packets;

UINT GCSouXiaDiscardHandler::Execute(GCSouXiaDiscard* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		switch ( pPacket->getResult() )
		{
		case DISCARDITEM_SUCCESS:
			{
				INT index = pPacket->getCurPos();

				CDataPool::GetMe()->UserSouXia_ClearInfo(index);
			}
			break;

		case DISCARDITEM_FAIL:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("ERASE_ITEM_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		}
		
	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}