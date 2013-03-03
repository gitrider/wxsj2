#include "StdAfx.h"
#include "GCSouXiaSew.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"
#include "..\..\Event\GMEventSystem.h"
#include "ResultDefine.h"

using namespace Packets;

UINT GCSouXiaSewHandler::Execute(GCSouXiaSew* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		switch( pPacket->GetResult() )
		{
		case SOUXIA_SEW_SUCCESS:		//·ìºÏ³É¹¦
			{
				SOUXIA_DATA* pSouxiaData = new SOUXIA_DATA;
				
				pSouxiaData->SetCurPos( pPacket->GetSouXiaPos() );
				
				//ËÑÏÀÂ¼¼¼ÄÜ
				pSouxiaData->m_Skill.StudyCount = pPacket->GetSouXiaSkillCount();
				for ( int i=0; i<pSouxiaData->m_Skill.StudyCount;++i )
				{
					pSouxiaData->m_Skill.StudySkillId[i] = pPacket->GetSouXiaSkillID(i);
				}

				//ËÑÏÀÂ¼ÎäÆ÷Åä·½
				pSouxiaData->m_Product.StudyCount = pPacket->GetSouXiaProductCount();
				for ( int i=0; i<pSouxiaData->m_Product.StudyCount; ++i )
				{
					pSouxiaData->m_Product.StudyProductId[i] = pPacket->GetSouXiaSProductID(i);
				}

				//ÉñÊÞÕÙ»½¼¼ÄÜ
				pSouxiaData->m_Pet.StudyCount = pPacket->GetSouXiaPetZhaoHuanCount();
				for ( int i=0; i<pSouxiaData->m_Pet.StudyCount; ++i )
				{
					pSouxiaData->m_Pet.StudyPet[i].StudyZhaoHuan = pPacket->GetSouXiaPetZhaoHuan(i).StudyZhaoHuan;
					pSouxiaData->m_Pet.StudyPet[i].LeftUseTime = pPacket->GetSouXiaPetZhaoHuan(i).LeftUseTime;
				}

				//×øÆïÕÙ»½¼¼ÄÜ
				pSouxiaData->m_ZuoJi.StudyCount = pPacket->GetSouXiaZuoJiZhaoHuanCount();
				for ( int i=0; i<pSouxiaData->m_ZuoJi.StudyCount; ++i )
				{
					pSouxiaData->m_ZuoJi.StudyZuoji[i].StudyZhaoHuan = pPacket->GetSouXiaZuoJiZhaoHuan(i).StudyZhaoHuan;
					pSouxiaData->m_ZuoJi.StudyZuoji[i].LeftUseTime = pPacket->GetSouXiaZuoJiZhaoHuan(i).LeftUseTime;
				}


				//memcpy( pSouxiaData, pPacket->GetSouXiaData(), sizeof(SOUXIA_DATA) );
				CDataPool::GetMe()->UserSouXia_SetInfo(pSouxiaData->GetCurPos(), pSouxiaData );
				CEventSystem::GetMe()->PushEvent(GE_SHOW_SOUXIASEW_BUILD );
			}
			break;

		case SOUXIA_SKILL_SEW_FAIL:		//·ìºÏÊ§°Ü
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_SEW_FAIL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_SKILL_PAGE_FULL:		//¼¼ÄÜÒ³ÂúÁË
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_SKILL_PAGE_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PRODUCT_PAGE_FULL:		//Åä·½Ò³ÂúÁË
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PRODUCT_PAGE_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_PET_ZHAOHUAN_PAGE_FULL:		//ÉñÊÞÕÙ»½Ò³ÂúÁË
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_PET_ZHAOHUAN_PAGE_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		case SOUXIA_ZUOJI_ZHAOHUAN_PAGE_FULL:	//×øÆïÕÙ»½Ò³ÂúÁË
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC("SOUXIA_ZUOJI_ZHAOHUAN_PAGE_FULL");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,strTemp.c_str());
			}
			break;

		}

		
	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}