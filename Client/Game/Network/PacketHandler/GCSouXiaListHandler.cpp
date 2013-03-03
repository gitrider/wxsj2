#include "StdAfx.h"
#include "GCSouXiaList.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDataPool.h"
#include "Object\Manager\ObjectManager.h"

using namespace Packets;

UINT GCSouXiaListHandler::Execute(GCSouXiaList* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION


	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		CDataPool::GetMe()->UserSouXia_SetCount( pPacket->GetSouXiaCount() );
		//CDataPool::GetMe()->UserSouXia_SetTypeCount( pPacket->GetSouXiaTypeCount() );
		
		int cout = pPacket->GetSouXiaCount();

		for ( int i=0; i<cout; ++i )
		{
			//SOUXIA_DATA * pSouxiaData = pPacket->GetSouXiaData(i);
			//SOUXIA_DATA* pSouxiaData = new SOUXIA_DATA;
			//memcpy(pSouxiaData, pPacket->GetSouXiaData(i), sizeof(SOUXIA_DATA));
			//CDataPool::GetMe()->UserSouXia_SetInfo( i, pSouxiaData );

			if( !(pPacket->GetSouXiaData(i)) ) continue;
			
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
	}
	return PACKET_EXE_CONTINUE;
	__LEAVE_FUNCTION
	return	PACKET_EXE_ERROR;
}