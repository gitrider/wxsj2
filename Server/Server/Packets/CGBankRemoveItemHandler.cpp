

#include "stdafx.h"
#include "CGBankRemoveItem.h"
#include "GCBankRemoveItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"

uint CGBankRemoveItemHandler::Execute( CGBankRemoveItem* pPacket, Player* pPlayer )
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

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	UINT indexFrom = pPacket->GetIndexFrom();
	UINT indexTo   = pPacket->GetIndexTo();
	
	ItemContainer* pBankContainer = pHuman->GetBankContain();
	if (indexFrom>(UINT)pBankContainer->GetContainerSize())
	{
		return PACKET_EXE_ERROR;
	}

	Item *pBankItem = HumanItemLogic::GetBankItem(pHuman, indexFrom);
	if(pBankItem->IsEmpty())
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: indexFrom=%d, indexTo=%d too many ask", indexFrom, indexTo ) ;
		return PACKET_EXE_CONTINUE;
	}

	GCBankRemoveItem Msg;

	switch(pPacket->GetToType())
	{
	case 	CGBankRemoveItem::EQUIP_POS:
		{
			/*if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
			{
				if (indexTo<HEQUIP_RIDER)
				{
					pHuman->GetDB()->OverWriteBank2Equip((HUMAN_EQUIP)indexTo, indexFrom);
					Msg.SetToType(GCBankRemoveItem::EQUIP_POS);
					Msg.SetIndexFrom(indexFrom);
					Msg.SetIndexTo(indexTo);
				}
			}*/
		}
		break;
	case 	CGBankRemoveItem::BAG_POS:		//Bank->Bag
		{

			//自动搜索空格
			if(indexTo == CGBankRemoveItem::AUTO_POS_BAG)
			{
				Item* pItem = HumanItemLogic::GetBankItem(pHuman, indexFrom);
				//验证位置里是否有东西
				if(ITEMREFPTRISVALID(pItem))
				{
					
					INT nLayNum = pItem->GetLayedNum();
					
					//HumanItemLogic::RecieveItemToBag(pHuman, pBankContainer, indexFrom);
			
					INT result 	= -1;
					ITEM_LOG_PARAM ItemLogParam;
					BOOL Ret = HumanItemLogic::MoveItem(&ItemLogParam, pHuman, pBankContainer, indexFrom, result);
					if(Ret)
					{
						ItemContainer* pBagContainer = HumanItemLogic::GetBagContainer(pHuman, result);
						Item* pDestItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(result));
						if (nLayNum<pDestItem->GetLayedNum())
						{
							Msg.SetToType(GCBankRemoveItem::BAG_POS);
							Msg.SetIndexFrom(indexFrom);
							Msg.SetIndexTo(result);
							Msg.SetOperateType(GCBankRemoveItem::OPERATE_SPLICE);
						}
						else
						{
							Msg.SetToType(GCBankRemoveItem::BAG_POS);
							Msg.SetIndexFrom(indexFrom);
							Msg.SetIndexTo(result);
							Msg.SetOperateType(GCBankRemoveItem::OPERATE_MOVE);
						}
					}
					else
					{
						Msg.SetToType(GCBankRemoveItem::ERROR_POS);
					}
				}
				else
				{
					Msg.SetToType(GCBankRemoveItem::ERROR_POS);
				}
			}

			//验证位置的合法性
			else if( indexTo>=0
				&& indexTo<MAX_BAG_SIZE 
				&& indexFrom>=0 
				&& indexFrom<(UINT)(pHuman->__GetBankEndIndex()) 
				)
			{
				if (indexTo>=EXTRA_CONTAINER_OFFSET)
				{
					return PACKET_EXE_CONTINUE;
				}

				//验证位置里是否有东西
				if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
				{
					//验证这个物品是不是能够存入指定的位置，
					Item *pItem = pBankContainer->GetItem(indexFrom); 	

					if(ITEMREFPTRISVALID(HumanItemLogic::GetBagItem(pHuman, indexTo)))
					{
						ItemContainer*	pBagContainer =
							HumanItemLogic::GetBagContainer(pHuman,indexTo);
						if (!pBagContainer->IsCanUse())
						{
							Msg.SetToType(GCBankRemoveItem::ERROR_POS);
							pGamePlayer->SendPacket(&Msg);

							g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: pos error, indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;
							return PACKET_EXE_CONTINUE ;
						}
						Item* pDestItem = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(indexTo));
						if (pDestItem->GetItemTableIndex() == pItem->GetItemTableIndex())
						{
							UINT nMaxLayNum = pDestItem->GetMaxLayedNum();
							UINT nCurLayNum = pDestItem->GetLayedNum();
							if (nMaxLayNum>1)
							{
								if (nCurLayNum<nMaxLayNum)
								{
									INT result 	= 
										g_ItemOperator.MoveItem(pBankContainer,
										(UINT)indexFrom,
										pBagContainer,
										indexTo);
									if (result>=0)
									{
										Msg.SetToType(GCBankRemoveItem::BAG_POS);
										Msg.SetIndexFrom(indexFrom);
										Msg.SetIndexTo(pBagContainer->ConIndex2BagIndex(result));
										Msg.SetOperateType(GCBankRemoveItem::OPERATE_SPLICE);
									}
								}
							}
							else
							{
								INT nResult = g_ItemOperator.ExchangeItem(pBagContainer,
									pBagContainer->BagIndex2ConIndex(indexTo),
									pBankContainer,
									(UINT)indexFrom);
								if (nResult<0)
								{
									Msg.SetToType(GCBankRemoveItem::ERROR_POS);
									pGamePlayer->SendPacket(&Msg);

									g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: pos error, indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;
									return PACKET_EXE_CONTINUE ;
								}
								Msg.SetOperateType(GCBankRemoveItem::OPERATE_SWAP); // 交换物品
							}
						}
						else
						{
							INT nResult = g_ItemOperator.ExchangeItem(pBagContainer,
								pBagContainer->BagIndex2ConIndex(indexTo),
								pBankContainer,
								(UINT)indexFrom);
							if (nResult<0)
							{
								Msg.SetToType(GCBankRemoveItem::ERROR_POS);
								pGamePlayer->SendPacket(&Msg);

								g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: pos error, indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;
								return PACKET_EXE_CONTINUE ;
							}
							Msg.SetOperateType(GCBankRemoveItem::OPERATE_SWAP); // 交换物品
						}
					}
					else
					{
						ItemContainer*	pBagContainer =
							HumanItemLogic::GetBagContainer(pHuman,indexTo);

						//ItemContainer* pBankContainer = pHuman->GetBankContain();

						INT nResult = g_ItemOperator.MoveItem(pBankContainer,
							(UINT)indexFrom,
							pBagContainer,
							pBagContainer->BagIndex2ConIndex(indexTo)
							);
						if (nResult<0)
						{
							Msg.SetToType(GCBankRemoveItem::ERROR_POS);
							pGamePlayer->SendPacket(&Msg);

							g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: pos error, indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;
							return PACKET_EXE_CONTINUE ;
						}
						Msg.SetOperateType(GCBankRemoveItem::OPERATE_MOVE);
					}

					//pHuman->GetDB()->SwapBankWithBag(indexTo, indexFrom);
					Msg.SetToType(GCBankRemoveItem::BAG_POS);
					Msg.SetIndexFrom(indexFrom);
					Msg.SetIndexTo(indexTo);
				}
				else
				{
					Msg.SetToType(GCBankRemoveItem::ERROR_POS);
				}
			}
			else
			{
				Msg.SetToType(GCBankRemoveItem::ERROR_POS);
			}
			
		}
		break;
	default:
		{
			return PACKET_EXE_ERROR;
		}
		break;
	}
	pGamePlayer->SendPacket(&Msg);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankRemoveItemHandler: indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
