#include "StdAfx.h"
#include "GCSouXiaUpdate.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Game\Interface\GMGameInterface_Script_SouXia.h"

using namespace Packets;

UINT GCSouXiaUpdateHandler::Execute(GCSouXiaUpdate* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		INT pos = pPacket->GetCurSouXiaPos();
		INT type = pPacket->GetSouXiaType();

		SOUXIA_DATA* pSouXiaData = CDataPool::GetMe()->UserSouXia_GetInfo(pPacket->GetCurSouXiaPos());

		switch(type)
		{
		case 1:
			{
				
			}
			break;
		case 2:
			{
				
			}
			break;
		case 3:
			{
				memcpy( &(pSouXiaData->m_Pet), pPacket->GetSouXiaPetZhaoHuan(), sizeof(SouXia_PetZhaoHuan));
			}
			break;
		case 4:
			{
				memcpy( &(pSouXiaData->m_ZuoJi), pPacket->GetSouXiaZuoJiZhaoHuan(), sizeof(SouXia_ZuojiZhaoHuan));
			}
			break;
		case 5:
			{
				
			}
			break;
		}
		CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_TAB );
		
	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}