#include "StdAfx.h"
#include "GCSouXiaRefining.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Interface\GMGameInterface_Script_SouXia.h"
#include "..\..\Game\Action\GMActionSystem.h"
#include "ResultDefine.h"

using namespace Packets;

UINT GCSouXiaRefiningHandler::Execute(GCSouXiaRefining* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		switch( pPacket->GetResult() )
		{
		case SOUXIA_REFINING_SUCCESS:
			{
				SOUXIA_DATA* pSouxiaData = new SOUXIA_DATA;
				
				pSouxiaData->SetCurPos( pPacket->GetSouXiaPos() );
				switch( pPacket->GetCurRefiningType() )
				{
				case ITEM_SOUXIASKILL:		//ËÑÏÀ¼¼ÄÜ
					{
						pSouxiaData->m_Skill.StudyCount = pPacket->GetSouXiaSkillCount();
						for ( int i=0; i<pSouxiaData->m_Skill.StudyCount; ++i )
						{
							pSouxiaData->m_Skill.StudySkillId[i] = pPacket->GetSouXiaSkillID(i);
						}
					}
					break;
				case ITEM_PRODUCT:		//ËÑÏÀÎäÆ÷Åä·½
					{
						pSouxiaData->m_Product.StudyCount = pPacket->GetSouXiaProductCount();
						for ( int i=0; i<pSouxiaData->m_Product.StudyCount; ++i )
						{
							pSouxiaData->m_Product.StudyProductId[i] = pPacket->GetSouXiaSProductID(i);
						}
					}
					break;
				case ITEM_PET_ZHAOHUAN:		//ËÑÏÀÉñÊÞÕÙ»½¼¼ÄÜ
					{
						pSouxiaData->m_Pet.StudyCount = pPacket->GetSouXiaPetZhaoHuanCount();
						for ( int i=0; i<pSouxiaData->m_Pet.StudyCount; ++i )
						{
							pSouxiaData->m_Pet.StudyPet[i].StudyZhaoHuan = pPacket->GetSouXiaPetZhaoHuan(i).StudyZhaoHuan;
							pSouxiaData->m_Pet.StudyPet[i].LeftUseTime = pPacket->GetSouXiaPetZhaoHuan(i).LeftUseTime;
						}
					}
					break;
				case ITEM_ZUOJI_ZHAOHUAN:		//ËÑÏÀ×øÆïÕÙ»½¼¼ÄÜ
					{
						pSouxiaData->m_ZuoJi.StudyCount = pPacket->GetSouXiaZuoJiZhaoHuanCount();
						for ( int i=0; i<pSouxiaData->m_ZuoJi.StudyCount; ++i )
						{
							pSouxiaData->m_ZuoJi.StudyZuoji[i].StudyZhaoHuan = pPacket->GetSouXiaZuoJiZhaoHuan(i).StudyZhaoHuan;
							pSouxiaData->m_ZuoJi.StudyZuoji[i].LeftUseTime = pPacket->GetSouXiaZuoJiZhaoHuan(i).LeftUseTime;
						}
					}
					break;
				}

				//memcpy( pSouxiaData, pPacket->GetSouXiaData(), sizeof(SOUXIA_DATA) );

				CDataPool::GetMe()->UserSouXia_SetInfo(pSouxiaData->GetCurPos(), pSouxiaData );

				INT pos = pPacket->GetNewItemBagIndex();

				INT ActionId = SCRIPT_SANDBOX::SouXiaLu::s_SouXiaLu.GetSouXiaEnumActionId(pos,NAMETYPE_PACKAGEITEM);
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SOUXIACOMPLEX_BUILD, ActionId );		
			}
			break;

		case SOUXIA_SKILL_REFINING_FAIL:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_REFINING_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_EMPTY:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_EMPTY");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		}


	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}