#include "stdafx.h"
#include "CGPackage_SwapItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCPackage_SwapItem.h"

#include "GCItemInfo.h"

uint CGPackage_SwapItemHandler :: Execute( CGPackage_SwapItem* pPacket, Player* pPlayer ) 
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
		return PACKET_EXE_ERROR;
	}

	//����߳�ִ����Դ�Ƿ���ȷ
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	//�õ���Ҫ��������������
	INT index1 = pPacket->GetPackageIndex1();
	INT index2 = pPacket->GetPackageIndex2();

	INT bSucc = -1;
	GCPackage_SwapItem msg;
	//��չ������ ��ֹ
	if ((index1>=EXTRA_CONTAINER_OFFSET)&&(index2>=EXTRA_CONTAINER_OFFSET))
	{
		msg.SetResult(SWITCHITEM_EXTRBAG_EXCHANGE_FAIL);
		pGamePlayer->SendPacket(&msg);
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d extrabag fail", 
			index1, index2) ;
		return PACKET_EXE_CONTINUE;
	}
	if (index1>=EXTRA_CONTAINER_OFFSET)
	{
		ItemContainer* pExtraBagContainer = HumanItemLogic::GetBagByExtraContainPos(pHuman, index1);
		//λ����Ч
		if (pExtraBagContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, is invalid", index1) ;
			return PACKET_EXE_ERROR;
		}
		//��չ��������
		if (!pExtraBagContainer->IsValid())
		{
			msg.SetResult(SWITCHITEM_SOURCEEXTRBAG_NOALIVE);
			pGamePlayer->SendPacket(&msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, extrabag is not alive", index1) ;
			return PACKET_EXE_CONTINUE;
		}
		//�ǿ���չ�������ƶ�
		if (!pExtraBagContainer->IsEmpty())
		{
			msg.SetResult(SWITCHITEM_SOURCEEXTRBAG_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, extrabag has items", index1) ;
			return PACKET_EXE_CONTINUE;
		}
		ItemContainer* pExtraContainer = pHuman->GetExtraContain();

		INT index_container1 = pExtraContainer->BagIndex2ConIndex(index1);
		
		ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, index2);
		INT index_container2 = pBagContainer->BagIndex2ConIndex(index2);
		
		if (pExtraBagContainer->IsInContainer(index2))
		{
			return PACKET_EXE_CONTINUE; 
		}
		Item* pItem2 = pBagContainer->GetItem(index_container2);

		//���ܽ���չ���ƶ�������Ʒλ��
		if (!pItem2->IsEmpty())
		{
			msg.SetResult(SWITCHITEM_SOURCEEXTRBAG_ITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, extrabag has items", index1) ;
			return PACKET_EXE_CONTINUE;
		}
		
		bSucc = g_ItemOperator.MoveItem(pExtraContainer, index_container1, pBagContainer, index_container2);
		if (bSucc>=0)
		{
			pHuman->ReInitExtraBag();
			msg.SetResult(SWITCHITEM_SUCCESS);
			msg.SetPackageIndex1(index1);
			msg.SetPackageIndex2(index2);

			pGamePlayer->SendPacket(&msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d Success", 
				index1, index2 ) ;
			return PACKET_EXE_CONTINUE;
		}
	}
	if ((index2>=EXTRA_CONTAINER_OFFSET)&&(index2<BASE_BAG_POS))
	{
		ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, index1);
		Item* pItem1 = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(index1));
		if (pItem1->IsEmpty())
		{
			return PACKET_EXE_CONTINUE;
		}
		ItemContainer* pExtraContainer = pHuman->GetExtraContain();

		//����Ʒ������չ��
		ItemContainer* pExtraBagContainer = HumanItemLogic::GetBagByExtraContainPos(pHuman, index2);
		if (pExtraBagContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index2=%d, is invalid", index2) ;
			return PACKET_EXE_ERROR;
		}

		INT index_container1 = pBagContainer->BagIndex2ConIndex(index1);
		INT index_container2 = pExtraContainer->BagIndex2ConIndex(index2);

		//��չ��������
		if (!pExtraBagContainer->IsValid())
		{
			if (pItem1->GetItemClass() == ICLASS_EXTRABAG)
			{
				INT ret = g_ItemOperator.MoveItem(pBagContainer, index_container1, 
					pExtraContainer, index_container2);
				if (ret>=0)
				{
					msg.SetResult(SWITCHITEM_SUCCESS);
					msg.SetPackageIndex1(index1);
					msg.SetPackageIndex2(index2);
					pGamePlayer->SendPacket(&msg);
					
					Item* pDestItem = pExtraContainer->GetItem(index_container2);
					Assert(pDestItem);
					if (pDestItem->GetExtraBagBeginTime() == INVALID_TIME)
					{
						pDestItem->SetExtraBagBeginTime();
					}
					pHuman->ReInitExtraBag();

					g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: use extrabagitem, index2=%d ", index2 ) ;
					return PACKET_EXE_CONTINUE;
				}
				msg.SetResult(SWITCHITEM_FAIL);
				pGamePlayer->SendPacket(&msg);
				return PACKET_EXE_CONTINUE;
			}
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index2=%d, can't put", index2) ;
			return PACKET_EXE_CONTINUE;
		}
		if (pExtraBagContainer->IsInContainer(index1))
		{
			return PACKET_EXE_CONTINUE;
		}
		if (pExtraBagContainer->IsCanUse())
		{
			INT nBagIndex = -1;
			nBagIndex =g_ItemOperator.MoveItem(pBagContainer, index_container1, pExtraBagContainer, -1);
			if (nBagIndex>=0)
			{
				msg.SetResult(SWITCHITEM_SUCCESS);
				msg.SetPackageIndex1(index1);
				msg.SetPackageIndex2(pExtraBagContainer->ConIndex2BagIndex(nBagIndex));
				pGamePlayer->SendPacket(&msg);
				g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d Success", 
					index1, index2 ) ;
				return PACKET_EXE_CONTINUE;
			}
			msg.SetResult(SWITCHITEM_FAIL);
			pGamePlayer->SendPacket(&msg);
			return PACKET_EXE_CONTINUE;
		}
		msg.SetResult(SWITCHITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	if (index2 == BASE_BAG_POS)
	{
		if (index1<MAX_SINGLEBAG_SIZE)
		{
			return PACKET_EXE_CONTINUE;
		}
		
		ItemContainer* pBagContainer = pHuman->GetBaseContain();
		ItemContainer* pExtraBagContainer = HumanItemLogic::GetBagContainer(pHuman, index1);
		if (pExtraBagContainer == NULL)
		{
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d fail", 
				index1, index2 ) ;
			return PACKET_EXE_ERROR;
		}
		INT index_container1 = pExtraBagContainer->BagIndex2ConIndex(index1);
		INT nBagIndex = -1;
		nBagIndex =g_ItemOperator.MoveItem(pExtraBagContainer, index_container1, pBagContainer, -1);
		if (nBagIndex>=0)
		{
			msg.SetResult(SWITCHITEM_SUCCESS);
			msg.SetPackageIndex1(index1);
			msg.SetPackageIndex2(pBagContainer->ConIndex2BagIndex(nBagIndex));
			pGamePlayer->SendPacket(&msg);
			g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d Success", 
				index1, index2 ) ;
			return PACKET_EXE_CONTINUE;
		}

		msg.SetResult(SWITCHITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
		
	}
	//������Ʒ�ײ㽻����Ʒ
	Item* pItem1 = HumanItemLogic::GetBagItem(pHuman, index1);
	Item* pItem2 = HumanItemLogic::GetBagItem(pHuman, index2);

	if ((pItem1==NULL)||(pItem2==NULL))
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d pos fail", 
			index1, index2) ;
		return PACKET_EXE_ERROR;
	}

	//�ж��Ƿ���ͬһ��������
	ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, index1);
	if(pBagContainer != HumanItemLogic::GetBagContainer(pHuman, index2)) return PACKET_EXE_CONTINUE;
	
	if (!pBagContainer->IsCanUse())
	{
		msg.SetResult(SWITCHITEM_SOURCEEXTRBAG_ITEM_FAIL);
		pGamePlayer->SendPacket(&msg);
		return PACKET_EXE_CONTINUE;
	}
	//ת��Index����
	INT index_container1 = pBagContainer->BagIndex2ConIndex(index1);
	INT index_container2 = pBagContainer->BagIndex2ConIndex(index2);

	//������Ʒ���ǿո�
	if(pItem1->IsEmpty() && pItem2->IsEmpty())
	{
		return PACKET_EXE_CONTINUE;
	}
	// Item1 -> Item2(EMPTY)
	else if(!(pItem1->IsEmpty()) && pItem2->IsEmpty())
	{
		bSucc = g_ItemOperator.MoveItem(pBagContainer, index_container1, pBagContainer, index_container2) >= 0;
	}
	// Item1(EMPTY) <- Item2
	else if(pItem1->IsEmpty() && !(pItem2->IsEmpty()))
	{
		bSucc = g_ItemOperator.MoveItem(pBagContainer, index_container2, pBagContainer, index_container1) >= 0;
	}
	// Item1 <-> Item2
	else
	{
		ITEM_LOG_PARAM	ItemLogParam;

		//�ϲ���ͬ����Ʒ
		//1���ж�������Ʒ�ǲ��ǿ��Ե��ӵ�
		if(pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && pItem1->IsCanLay())
		{
			int Count  = pItem1->GetLayedNum();
			//2��ת����Ʒ
			if(g_ItemOperator.MoveItem(pBagContainer, index_container1, index_container2) >= 0)
			{
				//�ϲ��ɹ��������֪ͨ�ͻ��ˣ�����ֻ����Index����Ҫ�������ݣ�
				_ITEM temp1;
				pItem1->SaveValueTo(&temp1);
				GCItemInfo msg1;
				msg1.setID(index1);
				msg1.setIsNull(pItem1->IsEmpty());
				msg1.setItem(&temp1);
				pGamePlayer->SendPacket(&msg1);

				_ITEM temp2;
				pItem2->SaveValueTo(&temp2);
				GCItemInfo msg2;
				msg2.setID(index2);
				msg2.setIsNull(pItem2->IsEmpty());
				msg2.setItem(&temp2);
				pGamePlayer->SendPacket(&msg2);


				ITEM_LOG_PARAM	ItemLogParam;
				ItemLogParam.OpType		= ITEM_TILED_BAG_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.ContainerPos		= index2;
				ItemLogParam.Count		= Count;
				ItemLogParam.ItemGuid	= pItem2->GetGUID();	
				SaveItemLog(&ItemLogParam);


				ItemLogParam.OpType		= ITEM_TILED_BAG_DEST;
				ItemLogParam.CharGUID	= pHuman->GetGUID();
				ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
				ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
				ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
				ItemLogParam.ContainerPos		= index1;
				ItemLogParam.Count		= Count;
				ItemLogParam.ItemGuid	= pItem1->GetGUID();	
				SaveItemLog(&ItemLogParam);

				return PACKET_EXE_CONTINUE ;
			}
		}

		// ��������Ʒ
		bSucc = g_ItemOperator.ExchangeItem(pBagContainer, index_container1, pBagContainer, index_container2) == ITEMOE_SUCCESS;
	}

	msg.SetResult(SWITCHITEM_SUCCESS);
	msg.SetPackageIndex1(index1);
	msg.SetPackageIndex2(index2);

	pGamePlayer->SendPacket(&msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGPackage_SwapItem: index1=%d, index2=%d Succ=%s", 
		index1, index2, bSucc?"TRUE":"FALSE" ) ;


	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

