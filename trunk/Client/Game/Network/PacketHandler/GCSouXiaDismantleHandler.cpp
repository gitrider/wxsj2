#include "StdAfx.h"
#include "GCSouXiaDismantle.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMGameInterface_Script_SouXia.h"
#include "..\..\Game\Action\GMActionSystem.h"

using namespace Packets;

UINT GCSouXiaDismantleHandler::Execute(GCSouXiaDismantle* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		switch( pPacket->GetResult() )
		{
		case SOUXIA_DISMANTLE_SUCCESS:
			{
				INT num = pPacket->GetNewSouXiaRelicCount();
				if ( num == 0 )
				{
					CEventSystem::GetMe()->PushEvent( GE_SHOW_SOUXIADISMANTLE_BUILD, -1, 0 );
				} 
				else
				{
					for ( int i=0; i<num; ++i )
					{
						SOUXIA_DATA* pSouxiaData = new SOUXIA_DATA;
						pSouxiaData->SetCurPos( pPacket->GetSouXiaPos(i) );
						//memcpy(pSouxiaData, pPacket->GetSouXiaData(i), sizeof(SOUXIA_DATA));

						//ËÑÏÀ¼¼ÄÜ
						pSouxiaData->m_Skill.StudyCount = pPacket->GetSouXiaSkillCount(i);
						for( int index= 0; index < pPacket->GetSouXiaSkillCount(i); ++index )
						{
							pSouxiaData->m_Skill.StudySkillId[index] = pPacket->GetSouXiaSkillID(i,index);
						}

						//ËÑÏÀÎäÆ÷Åä·½
						pSouxiaData->m_Product.StudyCount = pPacket->GetSouXiaProductCount(i);
						for ( int index = 0; index < pPacket->GetSouXiaProductCount(i); ++index )
						{
							pSouxiaData->m_Product.StudyProductId[index] = pPacket->GetSouXiaSProductID(i, index);
						}

						//ËÑÏÀÉñÊÞÕÙ»½¼¼ÄÜ
						pSouxiaData->m_Pet.StudyCount = pPacket->GetSouXiaPetZhaoHuanCount(i);
						for ( int index = 0; index < pPacket->GetSouXiaPetZhaoHuanCount(i); ++index )
						{
							pSouxiaData->m_Pet.StudyPet[index].StudyZhaoHuan = pPacket->GetSouXiaPetZhaoHuan(i,index).StudyZhaoHuan;
							pSouxiaData->m_Pet.StudyPet[index].LeftUseTime = pPacket->GetSouXiaPetZhaoHuan(i,index).LeftUseTime;
						}

						//ËÑÏÀ×øÆïÕÙ»½¼¼ÄÜ
						pSouxiaData->m_ZuoJi.StudyCount = pPacket->GetSouXiaZuoJiZhaoHuanCount(i);
						for ( int index = 0; index < pPacket->GetSouXiaZuoJiZhaoHuanCount(i); ++index )
						{
							pSouxiaData->m_ZuoJi.StudyZuoji[index].StudyZhaoHuan = pPacket->GetSouXiaZuoJiZhaoHuan(i, index).StudyZhaoHuan;
							pSouxiaData->m_ZuoJi.StudyZuoji[index].LeftUseTime = pPacket->GetSouXiaZuoJiZhaoHuan(i, index).LeftUseTime;
						}

						CDataPool::GetMe()->UserSouXia_SetInfo(pSouxiaData->GetCurPos(), pSouxiaData );
					}

					INT pos = pPacket->GetCurBagIndex(0);
					INT ActionId = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiaEnumActionId(pos,NAMETYPE_PACKAGEITEM );
					CEventSystem::GetMe()->PushEvent( GE_SHOW_SOUXIADISMANTLE_BUILD, ActionId, num );
				}
			}
			break;

		case SOUXIA_DISMANTLE_FAIL:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_DISMANTLE_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;
		}

	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}