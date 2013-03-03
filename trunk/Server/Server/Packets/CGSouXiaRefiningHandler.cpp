/********************************************************************
	created:	2010/5/19
	filename: 	MapServer\Server\Packets\CGSouXiaRefiningHandler.cpp
	file path:	MapServer\Server\Packets
	file base:	CGSouXiaRefiningHandler
	file ext:	cpp
	author:		genghao
	
	purpose:	��Ϣ�������
	
*********************************************************************/

#include "stdafx.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "CGSouXiaRefining.h"
#include "GCSouXiaRefining.h"
#include "SouXiaLogic.h"
#include "GCItemInfo.h"

UINT CGSouXiaRefiningHandler::Execute( CGSouXiaRefining* pPacket, Player* pPlayer )
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
	BYTE iRefiningSouXiaItem = pPacket->GetRefiningSouXiaIndex();
	Assert(iRefiningSouXiaItem >= 0 );
	Assert(iRefiningSouXiaItem <= MAX_BAG_SIZE);

	BYTE iRefiningSouXiaItem2 = pPacket->GetRefiningSouXia2Index();
	Assert(iRefiningSouXiaItem2 >= 0 );
	Assert(iRefiningSouXiaItem2 <= MAX_BAG_SIZE);


	BYTE iRefiningItem = pPacket->GetRefiningItemIndex();
	Assert(iRefiningItem >= 0 );
	Assert(iRefiningItem <= MAX_BAG_SIZE);

	GCSouXiaRefining  refiningMsg;
	// ����������ȡ����������Ϣ����֤
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningSouXiaItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia BagIndex is invalid, BagIndex = %d", iRefiningSouXiaItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningSouXiaItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem));
	if(pRefiningSouXiaItem->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pRefiningSouXiaItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia type is invalid, BagIndex = %d", iRefiningSouXiaItem) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);

		return PACKET_EXE_ERROR;
	}

	// ����������ȡ�ڶ�������¼������Ϣ
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningSouXiaItem2);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia BagIndex is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningSouXiaItem2 = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem2));
	if(pRefiningSouXiaItem2->GetItemClass() != ICLASS_EQUIP || EQUIP_SOUXIA != pRefiningSouXiaItem2->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining souxia type is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);
		return PACKET_EXE_ERROR;
	}
	
	// ����������ȡ����������Ϣ
	pBagContainer = HumanItemLogic::GetBagContainer(pHuman, iRefiningItem);
	if (pBagContainer == NULL)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining refiningItem BagIndex is invalid, BagIndex = %d", iRefiningItem) ;
		return PACKET_EXE_ERROR;
	}

	Item* pRefiningItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(iRefiningItem));
	if(pRefiningItem->GetItemClass() != ICLASS_COMITEM || COMITEM_SOUXIA != pRefiningItem->GetItemType())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGSouXiaRefining refiningItem type is invalid, BagIndex = %d", iRefiningSouXiaItem2) ;
		refiningMsg.SetResult(GET_ITEM_ERROR);
		pGamePlayer->SendPacket(&refiningMsg);
		return PACKET_EXE_ERROR;
	}

	SOUXIA_INFO_TB* pGet = g_ItemTable.GetSouXiaTB(pRefiningItem->GetItemTableIndex());
	if (NULL != pGet && ITEM_REFINING == pGet->m_nUseType)
	{
		if(pGet->m_nNeedLevel > pHuman->GetLevel()) // ʹ�õ���û�дﵽ���Ƶȼ�
		{
			refiningMsg.SetResult(HUMAN_LEVEL_NOT_ENOUGH);
			Player* pPlayer = pHuman->GetPlayer();
			if( pPlayer != NULL )
			{
				pPlayer->SendPacket( &refiningMsg );
			}

			return PACKET_EXE_CONTINUE;
		}
	}
	// �����߼�
	ItemParamValue ipv = (ItemParamValue)IPV_INT;
	INT	iParamSouXiaID = pRefiningSouXiaItem->GetItemParam(0,ipv);
	INT	iParamSouXiaPos = pRefiningSouXiaItem->GetItemParam(4,ipv);

	INT iParamSouXiaID2 = pRefiningSouXiaItem2->GetItemParam(0,ipv);
	INT iParamSouXiaPos2 = pRefiningSouXiaItem2->GetItemParam(4,ipv);
	//��������¼��Ϊ��
	if ( (iParamSouXiaID == 0 && iParamSouXiaPos <= 0 ) && (iParamSouXiaID2 == 0 && iParamSouXiaPos2 <= 0 ))
	{
		refiningMsg.SetResult(SOUXIA_EMPTY);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
		return PACKET_EXE_CONTINUE;
	}

	BOOL  bRefiningResult = FALSE;
	BYTE  refiningType = 0;
	BYTE  refiningRule = 0;
	SHORT refiningRate = -1;
	BYTE randNum = pScene->GetRand100();
	SOUXIA_DATA randSouXiaData;//���������ȡ�õ�������
	
	if (pGet->m_nRefiningRule[0][0] > 0)//�������ܵ���
	{
		refiningRule = pGet->m_nRefiningRule[0][0];
		refiningRate = pGet->m_nRefiningRule[0][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_SOUXIASKILL);
			refiningType = ITEM_SOUXIASKILL;
		}
	}
	else if (pGet->m_nRefiningRule[1][0] > 0)//�����䷽����
	{
		refiningRule = pGet->m_nRefiningRule[1][0];
		refiningRate = pGet->m_nRefiningRule[1][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_PRODUCT);
			refiningType = ITEM_PRODUCT;
		}
	}
	else if (pGet->m_nRefiningRule[2][0] > 0)//���������ٻ�����
	{
		refiningRule = pGet->m_nRefiningRule[2][0];
		refiningRate = pGet->m_nRefiningRule[2][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_PET_ZHAOHUAN);
			refiningType = ITEM_PET_ZHAOHUAN;
		}
	}
	else if (pGet->m_nRefiningRule[3][0] > 0)//���������ٻ�����
	{
		refiningRule = pGet->m_nRefiningRule[3][0];
		refiningRate = pGet->m_nRefiningRule[3][1];
		if (randNum < refiningRate)
		{
			bRefiningResult = SouXiaLogic::RefiningSouXia(pHuman, pRefiningSouXiaItem, pRefiningSouXiaItem2, refiningRule, randSouXiaData, ITEM_ZUOJI_ZHAOHUAN);
			refiningType = ITEM_ZUOJI_ZHAOHUAN;
		}
	} 

	if (!bRefiningResult)
	{
		refiningMsg.SetResult(SOUXIA_SKILL_REFINING_FAIL);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
		return PACKET_EXE_CONTINUE;
	}

	// �������õĿ۳�
	// �ɹ�ɾ��ʹ�õĵ��ߺ͒���¼
	if (randNum <= refiningRate)
	{
		UINT  newSouXiaIndex = 0;
		BYTE  newSouXiaCurPos = 0;

		// ȡ������ϒ���¼������pos �� skillId
		if (iParamSouXiaID == 0 && iParamSouXiaPos <= 0)//��һ��Ϊ��
		{
			newSouXiaIndex = iParamSouXiaID2;
			newSouXiaCurPos = iParamSouXiaPos2;
		} 
		if (iParamSouXiaID2 == 0 && iParamSouXiaPos2 <= 0)//�ڶ���Ϊ��
		{
			newSouXiaIndex = iParamSouXiaID;
			newSouXiaCurPos = iParamSouXiaPos;
		} 
		
		// ��һ�������ҳ��
		BYTE maxPage = 0, maxPage2 = 0;
		pGet = g_ItemTable.GetSouXiaTB(pRefiningSouXiaItem->GetItemTableIndex());
		Assert(pGet);
		maxPage = pGet->m_nMaxPages[0];
		// �ڶ������ҳ��
		pGet = g_ItemTable.GetSouXiaTB(pRefiningSouXiaItem2->GetItemTableIndex());
		Assert(pGet);
		maxPage2 = pGet->m_nMaxPages[0];

		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType	=	ITEM_REFINING_SOUXIA;
		UINT  newSouXiaBagIndex = 0;//�µĒ���¼item��������

		if (maxPage >= maxPage2) // ��һ�������ҳ���ڵڶ���������һ��
		{
			BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem2));
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			else 
			{
				refiningMsg.SetResult(ERASE_ITEM_FAIL);
				pGamePlayer->SendPacket(&refiningMsg);
				return PACKET_EXE_CONTINUE;
			}
			if (iParamSouXiaPos2 >= 0)
			{
				pHuman->EraseSouXia(iParamSouXiaPos2);//���������ٵڶ���
			}
			
			newSouXiaBagIndex = iRefiningSouXiaItem;//������һ�������ݵ�bagindex

			if ( (iParamSouXiaID > 0  && iParamSouXiaPos >= 0) && (iParamSouXiaID2 > 0  && iParamSouXiaPos2 >= 0))
			{
				newSouXiaIndex = iParamSouXiaID;
				newSouXiaCurPos = iParamSouXiaPos;
			}
		} 
		else // ���ٵ�һ��
		{
			BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningSouXiaItem));
			if(bRet)
			{
				SaveItemLog(&ItemLogParam);
			}
			else 
			{
				refiningMsg.SetResult(ERASE_ITEM_FAIL);
				pGamePlayer->SendPacket(&refiningMsg);
				return PACKET_EXE_CONTINUE;
			}

			if (iParamSouXiaPos >= 0)
			{
				pHuman->EraseSouXia(iParamSouXiaPos);//���������ٵ�һ��
			}
			newSouXiaBagIndex = iRefiningSouXiaItem2;//�����ڶ��������ݵ�bagindex

			if ( (iParamSouXiaID > 0  && iParamSouXiaPos >= 0) && (iParamSouXiaID2 > 0  && iParamSouXiaPos2 >= 0))
			{
				newSouXiaIndex = iParamSouXiaID2;
				newSouXiaCurPos = iParamSouXiaPos2;
			}
		}
		Assert (newSouXiaIndex > 0 && newSouXiaCurPos >= 0);
		randSouXiaData.m_CurPos = newSouXiaCurPos;
		randSouXiaData.m_SouXiaID = newSouXiaIndex;
		// ������������
		BOOL bRet = HumanItemLogic::EraseBagItem(&ItemLogParam,pHuman, pBagContainer->BagIndex2ConIndex(iRefiningItem));
		if(bRet)
		{
			SaveItemLog(&ItemLogParam);
		}
		else 
		{
			refiningMsg.SetResult(ERASE_ITEM_FAIL);
			pGamePlayer->SendPacket(&refiningMsg);
			return PACKET_EXE_CONTINUE;
		}
		// �ı䱣���Ē���¼���ߵ���Ϣ
		Item* pNewSouXiaItem = HumanItemLogic::GetBagItem(pHuman, pBagContainer->BagIndex2ConIndex(newSouXiaBagIndex));
		Assert(pNewSouXiaItem);

		bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex),0, ipv, newSouXiaIndex);
		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex), 4, ipv, newSouXiaCurPos);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}

		/*bRet = g_ItemOperator.SetItemParam( pHuman->GetBaseContain(),pHuman->GetBaseContain()->BagIndex2ConIndex(newSouXiaBagIndex),8, ipv, refiningType);

		if (!bRet)
		{
			return PACKET_EXE_CONTINUE;
		}*/

		// ��������ITEM paramע�Ᵽ�浽�ļ�,����Ҫ֪ͨITEM ��param �ı�
		GCItemInfo	Msg;
		Msg.setID( newSouXiaBagIndex );
		Msg.setIsNull(FALSE);
		pNewSouXiaItem->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);

		pHuman->SetSouXia(randSouXiaData, newSouXiaCurPos);
		// ���浽�ļ�
		_SOUXIA_DB_LOAD		SouXia_db_Load;
		SouXia_db_Load.m_SouXiaData[newSouXiaCurPos] = randSouXiaData;
		pHuman->GetDB()->SetSouXiaDBInfo(&SouXia_db_Load, newSouXiaCurPos);	
		
		// �����µĒ���¼��Ϣ
		refiningMsg.SetNewItemBagIndex(newSouXiaBagIndex);
		refiningMsg.SetSouXiaData(&randSouXiaData);//�����Ժ�Ē���¼����
		refiningMsg.SetCurRefiningType(refiningType);
		refiningMsg.SetResult(SOUXIA_REFINING_SUCCESS);
		Player* pPlayer = pHuman->GetPlayer();
		if( pPlayer != NULL )
		{
			pPlayer->SendPacket( &refiningMsg );
		}
	}	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return	PACKET_EXE_ERROR;
}

