/********************************************************************
created:	2010/5/20
filename: 	MapServer\Server\Packets\CGSouXiaSewHandler.cpp
file path:	MapServer\Server\Packets
file base:	CGSouXiaSewHandler
file ext:	cpp
author:		genghao

purpose:	��Ϣ�������

*********************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "CGSouXiaSew.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCSouXiaSew.h"
#include "GCItemInfo.h"

UINT CGSouXiaSewHandler::Execute( CGSouXiaSew* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	//�г���ʱ������װ��Ϣ
	//����״̬���ɲ���
	//��̯״̬���ɲ���

	// ��ȡ��ϵĒ���¼�ͷ�ϵ���
	BYTE iSouXiaItem = pPacket->GetSewSouXiaIndex();
	Assert(iSouXiaItem >= 0 );
	Assert(iSouXiaItem <= MAX_BAG_SIZE);

	BYTE iSewItem = pPacket->GetSewItemIndex();
	Assert(iSewItem >= 0 );
	Assert(iSewItem <= MAX_BAG_SIZE);


	BYTE iRelicItem = pPacket->GetRelicItemIndex();
	Assert(iRelicItem >= 0 );
	Assert(iRelicItem <= MAX_BAG_SIZE);

	GCSouXiaSew  sewMsg;
	// ����������ȡ����������Ϣ
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iSouXiaItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia BagIndex is invalid, BagIndex = %d", iSouXiaItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pSouXiaItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSouXiaItem));
	if(pSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pSouXiaItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia type is invalid, BagIndex = %d", iSouXiaItem) ;
		sewMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&sewMsg);
		return PACKET_EXE_ERROR;
	}

	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT	iParamSouXiaID = pSouXiaItem->GetItemParam(0,ipv);
	INT	iParamSouXiaPos = pSouXiaItem->GetItemParam(4,ipv);
	// ����������ȡ��ϵ�����Ϣ
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iSewItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew SewItem BagIndex is invalid, BagIndex = %d", iSewItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pSewItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iSewItem));
	if(pSewItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pSewItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew SewItem type is invalid, BagIndex = %d", iSewItem) ;
		sewMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&sewMsg);
		return PACKET_EXE_ERROR;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pSewItem->GetItemTableIndex());
	INT nSewRate = 0;
	if (NULL != pGet && ITEM_SEW == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // ʹ�õ���û�дﵽ���Ƶȼ�
		{
			sewMsg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);
			pGamePlayer->SendPacket(&sewMsg);
			return PACKET_EXE_CONTINUE;
		}
		nSewRate = pGet->m_nSewRate;
	}

	// ����������ȡ��ҳ������Ϣ
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRelicItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew relicItem BagIndex is invalid, BagIndex = %d", iRelicItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRelicItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRelicItem));
	if(pRelicItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA_RELIC != pRelicItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew relicItem type is invalid, BagIndex = %d", iRelicItem) ;
		sewMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&sewMsg);
		return PACKET_EXE_ERROR;
	}
	// ����߼�
	INT	iRelicSouXiaID = pRelicItem->GetItemParam(0,ipv);
	INT	iRelicSouXiaPos = pRelicItem->GetItemParam(4,ipv);
	INT	iRelicSouXiaType = pRelicItem->GetItemParam(8,ipv);
	Assert(iRelicSouXiaType >= ITEM_SOUXIASKILL && iRelicSouXiaType <= ITEM_ZUOJI_ZHAOHUAN);
	//ĸ����¼�����ҳ��
	BYTE curPage = 0, maxPage = 0;
	BYTE randNum = pScene->GetRand100();
	
	SOUXIA_DATA souXiaData;
	BOOL bEraseRelicPos = FALSE;//�Ƿ���Ҫ��������ɾ����ҳ�����ĸ����¼δ�ӵ���������ɾ��
	if (iParamSouXiaID > 0 && iParamSouXiaPos >= 0)
	{
		SouXia refiningSouXia = pHuman->GetSouXia(iParamSouXiaPos);// �������ϻ�ȡ������Ϣ
		souXiaData = refiningSouXia.GetSouXiaData();
		bEraseRelicPos = TRUE;
	} 

	pGet = g_ItemTable.GetSouXiaTB(pSouXiaItem->GetItemTableIndex());
	Assert(pGet);
	maxPage = pGet->m_nMaxPages[iRelicSouXiaType];// ĸ����¼���ҳ��Ӧ�ļ������͵����ҳ��

	if (iParamSouXiaPos < 0 || iParamSouXiaID == 0)
	{
		BOOL bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),
			pHuman->GetBaseContain()->BagIndex2ConIndex(iSouXiaItem),0, ipv, iRelicSouXiaID);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),
			pHuman->GetBaseContain()->BagIndex2ConIndex(iSouXiaItem),4, ipv, iRelicSouXiaPos);

		iParamSouXiaPos = iRelicSouXiaPos;
		iParamSouXiaID = iRelicSouXiaID;
		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}
		// ��������ITEM paramע�Ᵽ�浽�ļ�,����Ҫ֪ͨITEM ��param �ı�
		GCItemInfo	Msg;
		Msg.setID( iSouXiaItem );
		Msg.setIsNull(FALSE);
		Assert(pSouXiaItem);
		pSouXiaItem->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);

	}

	switch(iRelicSouXiaType)
	{	
	case ITEM_SOUXIASKILL:
		if (souXiaData.SkillIsFull())
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia Skill  is Full, BagIndex = %d", iSouXiaItem) ;
			sewMsg.SetResult(SOUXIA_SKILL_PAGE_FULL);
			pGamePlayer->SendPacket(&sewMsg);
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		//��ϼ���ҳ
		if (iRelicSouXiaID > 0 && iRelicSouXiaPos >= 0 && randNum <= nSewRate)
		{
			SouXia relicSouXia = pHuman->GetSouXia(iRelicSouXiaPos);
			SOUXIA_DATA relicSouxiaData = relicSouXia.GetSouXiaData();

			//��֤��Ϻ��ҳ���Ƿ�Ϸ�
			BYTE curRelicSkillCount = relicSouxiaData.m_Skill.StudyCount;
			BYTE pageCountAfterSew = souXiaData.GetCurSkillPage() + (BYTE)(((curRelicSkillCount + SKILL_PER_PAGE)-1)/SKILL_PER_PAGE);

			if (pageCountAfterSew > maxPage)
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia  is Full, BagIndex = %d", iSouXiaItem) ;
				
				sewMsg.SetResult(SOUXIA_SKILL_PAGE_FULL);
				pGamePlayer->SendPacket(&sewMsg);

				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}
			BYTE studySkillCount = 0;
			for (BYTE i=0; i<curRelicSkillCount; ++i)
			{
				studySkillCount = souXiaData.m_Skill.StudyCount;
				souXiaData.m_Skill.StudySkillId[studySkillCount] = relicSouxiaData.m_Skill.StudySkillId[i];
				souXiaData.m_Skill.StudyCount++;
			}
			souXiaData.m_CurPos = iParamSouXiaPos;
			souXiaData.m_SouXiaID = iParamSouXiaID;
			// ���浽�ļ�
			_SOUXIA_DB_LOAD		SouXia_db_Load;
			SouXia_db_Load.m_SouXiaData[iParamSouXiaPos] = souXiaData;
			pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iParamSouXiaPos);	

			//refiningSouXia.SetSouXiaData(souXiaData);
			pHuman->SetSouXia(souXiaData, iParamSouXiaPos); //��������
		}
		break;

	case ITEM_PRODUCT:
		{
			if (souXiaData.ProductIsFull())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia product zhaohuan  is Full, BagIndex = %d", iSouXiaItem) ;
				sewMsg.SetResult(SOUXIA_PRODUCT_PAGE_FULL);
				pGamePlayer->SendPacket(&sewMsg);
				return PACKET_EXE_CONTINUE;
			}
			//��������ٻ�����ҳ
			if (iRelicSouXiaID > 0 && iRelicSouXiaPos >= 0 && randNum <= nSewRate)
			{
				SouXia relicSouXia = pHuman->GetSouXia(iRelicSouXiaPos);
				SOUXIA_DATA relicSouxiaData = relicSouXia.GetSouXiaData();

				//��֤��Ϻ��ҳ���Ƿ�Ϸ�
				BYTE curRelicProductCount = relicSouxiaData.m_Product.StudyCount;
				BYTE pageCountAfterSew = souXiaData.GetCurProductPage() + (BYTE)(((curRelicProductCount + PRODUCT_PER_PAGE)-1)/PRODUCT_PER_PAGE);

				if (pageCountAfterSew > maxPage)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia  is Full, BagIndex = %d", iSouXiaItem) ;
					sewMsg.SetResult(SOUXIA_PRODUCT_PAGE_FULL);
					pGamePlayer->SendPacket(&sewMsg);
					return PACKET_EXE_CONTINUE;
				}
				
				BYTE studyProductSkillCount = 0;
				for (BYTE i=0; i<curRelicProductCount; ++i)
				{
					studyProductSkillCount = souXiaData.m_Product.StudyCount;
					souXiaData.m_Product.StudyProductId[studyProductSkillCount] = relicSouxiaData.m_Product.StudyProductId[i];
					souXiaData.m_Product.StudyCount++;
				}
				souXiaData.m_CurPos = iParamSouXiaPos;
				souXiaData.m_SouXiaID = iParamSouXiaID;
				// ���浽�ļ�
				_SOUXIA_DB_LOAD		SouXia_db_Load;
				SouXia_db_Load.m_SouXiaData[iParamSouXiaPos] = souXiaData;
				pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iParamSouXiaPos);	

				//refiningSouXia.SetSouXiaData(souXiaData);
				pHuman->SetSouXia(souXiaData, iParamSouXiaPos); //��������
			}
		}
		break;

	case ITEM_PET_ZHAOHUAN:
		{
			if (souXiaData.PetIsFull())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia pet zhaohuan  is Full, BagIndex = %d", iSouXiaItem) ;
				sewMsg.SetResult(SOUXIA_PET_ZHAOHUAN_PAGE_FULL);
				pGamePlayer->SendPacket(&sewMsg);
				return PACKET_EXE_CONTINUE;
			}
			//��������ٻ�����ҳ
			if (iRelicSouXiaID > 0 && iRelicSouXiaPos >= 0 && randNum <= nSewRate)
			{
				SouXia relicSouXia = pHuman->GetSouXia(iRelicSouXiaPos);
				SOUXIA_DATA relicSouxiaData = relicSouXia.GetSouXiaData();

				//��֤��Ϻ��ҳ���Ƿ�Ϸ�
				BYTE curRelicPetCount = relicSouxiaData.m_Pet.StudyCount;
				BYTE pageCountAfterSew = souXiaData.GetCurPetZhaoHuanPage() + (BYTE)(((curRelicPetCount + PET_ZHAOHUAN_PER_PAGE)-1)/PET_ZHAOHUAN_PER_PAGE);

				if (pageCountAfterSew > maxPage)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia  is Full, BagIndex = %d", iSouXiaItem) ;
					sewMsg.SetResult(SOUXIA_PET_ZHAOHUAN_PAGE_FULL);
					pGamePlayer->SendPacket(&sewMsg);
					return PACKET_EXE_CONTINUE;
				}

				BYTE studyPetSkillCount = 0;
				for (BYTE i=0; i<curRelicPetCount; ++i)
				{
					studyPetSkillCount = souXiaData.m_Pet.StudyCount;
					souXiaData.m_Pet.StudyPet[studyPetSkillCount].StudyZhaoHuan = relicSouxiaData.m_Pet.StudyPet[i].StudyZhaoHuan;
					souXiaData.m_Pet.StudyPet[studyPetSkillCount].LeftUseTime = relicSouxiaData.m_Pet.StudyPet[i].LeftUseTime;
					souXiaData.m_Pet.StudyCount++;
				}
				souXiaData.m_CurPos = iParamSouXiaPos;
				souXiaData.m_SouXiaID = iParamSouXiaID;
				// ���浽�ļ�
				_SOUXIA_DB_LOAD		SouXia_db_Load;
				SouXia_db_Load.m_SouXiaData[iParamSouXiaPos] = souXiaData;
				pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iParamSouXiaPos);	

				//refiningSouXia.SetSouXiaData(souXiaData);
				pHuman->SetSouXia(souXiaData, iParamSouXiaPos); //��������
			}
		}
		break;

	case ITEM_ZUOJI_ZHAOHUAN:
		{
			if (souXiaData.ZuoJiIsFull())
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia ZuoJi zhaohuan  is Full, BagIndex = %d", iSouXiaItem) ;
				sewMsg.SetResult(SOUXIA_ZUOJI_ZHAOHUAN_PAGE_FULL);
				pGamePlayer->SendPacket(&sewMsg);
				return PACKET_EXE_CONTINUE;
			}
			//��������ٻ�����ҳ
			if (iRelicSouXiaID > 0 && iRelicSouXiaPos >= 0 && randNum <= nSewRate)
			{
				SouXia relicSouXia = pHuman->GetSouXia(iRelicSouXiaPos);
				SOUXIA_DATA relicSouxiaData = relicSouXia.GetSouXiaData();

				//��֤��Ϻ��ҳ���Ƿ�Ϸ�
				BYTE curRelicZuoJiCount = relicSouxiaData.m_ZuoJi.StudyCount;
				BYTE pageCountAfterSew = souXiaData.GetCurZuoJiZhaoHuanPage() + (BYTE)(((curRelicZuoJiCount + ZUOJI_ZHAOHUAN_PER_PAGE)-1)/ZUOJI_ZHAOHUAN_PER_PAGE);

				if (pageCountAfterSew > maxPage)
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaSew souxia  is Full, BagIndex = %d", iSouXiaItem) ;
					sewMsg.SetResult(SOUXIA_ZUOJI_ZHAOHUAN_PAGE_FULL);
					pGamePlayer->SendPacket(&sewMsg);
					return PACKET_EXE_CONTINUE;
				}
				
				BYTE studyZuoJiSkillCount = 0;
				for (BYTE i=0; i<curRelicZuoJiCount; ++i)
				{
					studyZuoJiSkillCount = souXiaData.m_ZuoJi.StudyCount;
					souXiaData.m_ZuoJi.StudyZuoji[studyZuoJiSkillCount].StudyZhaoHuan = relicSouxiaData.m_ZuoJi.StudyZuoji[i].StudyZhaoHuan;
					souXiaData.m_ZuoJi.StudyZuoji[studyZuoJiSkillCount].LeftUseTime = relicSouxiaData.m_ZuoJi.StudyZuoji[i].LeftUseTime;
					souXiaData.m_ZuoJi.StudyCount++;
				}
				souXiaData.m_CurPos = iParamSouXiaPos;
				souXiaData.m_SouXiaID = iParamSouXiaID;
				// ���浽�ļ�
				_SOUXIA_DB_LOAD		SouXia_db_Load;
				SouXia_db_Load.m_SouXiaData[iParamSouXiaPos] = souXiaData;
				pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, iParamSouXiaPos);	

				//refiningSouXia.SetSouXiaData(souXiaData);
				pHuman->SetSouXia(souXiaData, iParamSouXiaPos); //��������
			}
		}
		break;

	default:
		break;

	}

	// �������õĿ۳�
	// �ɹ�ɾ��ʹ�õķ�ϵ��ߺͲ�ҳ����
	if (randNum <= nSewRate)
	{
		ITEM_LOG_PARAM		ItemLogParam;
		ItemLogParam.OpType	= ITEM_SEW_SOUXIA;
		// ��ϳɹ���ɾ����ϵ���
		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iSewItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else 
		{
			sewMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&sewMsg);
			return PACKET_EXE_CONTINUE;
		}
		// ���ٲ�ҳ����
		bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRelicItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else 
		{
			sewMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&sewMsg);
			return PACKET_EXE_CONTINUE;
		}
		if ( bEraseRelicPos && iRelicSouXiaPos >= 0)
		{
			if(!pHuman->EraseSouXia(iRelicSouXiaPos))
			{
				sewMsg.SetResult(ERASE_ITEM_FAIL);
				pGamePlayer->SendPacket(&sewMsg);
				return PACKET_EXE_CONTINUE;
			}
		}
		// �����µĒ���¼

		// ���ͷ�Ϻ��µĒ���¼��Ϣ
		if (iParamSouXiaPos >= 0 && iParamSouXiaID > 0)
		{
			SOUXIA_DATA	souXiaData = pHuman->GetSouXia(iParamSouXiaPos).GetSouXiaData();
			Assert(souXiaData.m_CurPos >=0 && souXiaData.m_SouXiaID > 0);
			sewMsg.SetSouXiaData(&souXiaData);
			sewMsg.SetResult(SOUXIA_SEW_SUCCESS);
			pGamePlayer->SendPacket(&sewMsg);
		}
	}
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return	PACKET_EXE_ERROR;
}

