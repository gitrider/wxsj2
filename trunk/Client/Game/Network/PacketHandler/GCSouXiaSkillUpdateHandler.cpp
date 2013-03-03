#include "StdAfx.h"
#include "GCSouXiaSkillUpdate.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Game\Interface\GMGameInterface_Script_SouXia.h"

using namespace Packets;

UINT GCSouXiaSkillUpdateHandler::Execute(GCSouXiaSkillUpdate* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		SOUXIA_DATA* pSouXiaData = CDataPool::GetMe()->UserSouXia_GetInfo(pPacket->GetCurSouXiaPos());

		BYTE type = pPacket->GetSouXiaSkillType();
		switch(type)
		{
		case 3:
			{
				int num = pSouXiaData->GetCurPetCount();
				for( int i=0; i<num; ++i )
				{
					if( pSouXiaData->m_Pet.StudyPet[i].StudyZhaoHuan == pPacket->GetSouXiaSkillID() )
					{
						pSouXiaData->m_Pet.StudyPet[i].LeftUseTime--;
						break;
					}
				}
			}
			break;

		case 4:
			{
				int num = pSouXiaData->GetCurZuoJiCount();
				for ( int i=0; i<num; ++i )
				{
					if( pSouXiaData->m_ZuoJi.StudyZuoji[i].StudyZhaoHuan == pPacket->GetSouXiaSkillID() )
					{
						pSouXiaData->m_ZuoJi.StudyZuoji[i].LeftUseTime--;
						break;
					}
				}
			}
			break;
		}

		SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.UpdateMainMenuBarSouxiaSkillIDNum(pPacket->GetSouXiaSkillID(), pPacket->GetSouXiaSkillTime() );
		CEventSystem::GetMe()->PushEvent( GE_UPDATE_SOUXIA_TAB );

		
	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}