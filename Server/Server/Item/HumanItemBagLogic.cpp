/********************************************************************
创建日期:	2008年3月17日
文件名称:	HumanItemBagLogic.cpp
创建人:		李珣

文件功能： 包裹相关逻辑操作
修改纪录：

*********************************************************************/
#include "stdafx.h"
#include "HumanItemLogic.h"
#include "Player.h"
#include "Item.h"
#include "ItemOperator.h"
#include "GCItemInfo.h"
#include "ItemRuler.h"
#include "GCNotifyEquip.h"
#include "Obj_Human.h"
#include "LogDefine.h"
#include "Scene.h"

INT	HumanItemLogic::EraseBagItem(ITEM_LOG_PARAM* pLogParam,
									 Obj_Human*	pHuman,ItemContainer* pContainer, UINT uItemIndex, INT uCount)
{
	__ENTER_FUNCTION

		Assert(uItemIndex!=INVALID_ID);
	Assert(uItemIndex>0);
	Assert(uCount>0);
	Assert(pHuman);
	Assert(pContainer);
	Assert(pLogParam);
	Assert(pContainer->IsValid());
	Item*				pItem;
	INT	i;

	for(i=0; i<pContainer->GetContainerSize() && uCount>0; ++i)
	{
		pItem		=	pContainer->GetItem(i);
		Assert( pItem != NULL );
		if( pItem->GetItemTableIndex() == uItemIndex )
		{
			if( pItem->IsLock())
			{
				continue;
			}
			if( pItem->GetMaxLayedNum() == 1 )
			{
				BOOL bRet =	EraseBagItem(pLogParam,pHuman,i);
				Assert(bRet);

				SaveItemLog(pLogParam);
				--uCount;
			}
			else
			{
				do
				{					
					g_ItemOperator.DecItemLayCount(pContainer, i, 1);
					--uCount;

				} while(pItem->GetLayedNum()>0 && uCount>0);

				if( pItem->GetLayedNum() <= 0 )
				{
					BOOL bRet	= EraseBagItem(pLogParam,pHuman,pContainer->ConIndex2BagIndex(i));
					Assert(bRet);
					SaveItemLog(pLogParam);
				}
				else
				{
					GCItemInfo Msg;
					pItem->SaveValueTo(Msg.getItem());
					Msg.setID(i);
					Msg.setIsNull(FALSE);
					pHuman->GetPlayer()->SendPacket(&Msg);
				}
			}
		}
	}
	
	return uCount;

	__LEAVE_FUNCTION

		return uCount ;
}

BOOL	HumanItemLogic::EraseBagItem(ITEM_LOG_PARAM* pLogParam,
									 Obj_Human* pHuman,UINT uItemIndex,UINT uCount)
{
	__ENTER_FUNCTION

		Assert(uItemIndex!=INVALID_ID);
	Assert(uItemIndex>0);
	Assert(uCount>0);
	Assert(pHuman);
	Assert(pLogParam);

	if(uCount<0)
	{
		return	FALSE;
	}

	ItemContainer*		pContainer;
	
	pContainer = pHuman->GetBaseContain();
	Assert(pContainer);
	
	INT nRemainCount = EraseBagItem(pLogParam, pHuman, pContainer, uItemIndex, uCount);

	if(nRemainCount>0)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pContainer = pHuman->GetExtraBagContain(i);
			Assert(pContainer);
			if(pContainer->IsValid())
			{
				nRemainCount = EraseBagItem(pLogParam, pHuman, pContainer, uItemIndex, uCount);
			}
			
			if (nRemainCount<=0)
			{
				break;
			}
		} 
	}
	
	if(nRemainCount>0)
		return FALSE;

	return TRUE;
	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL	HumanItemLogic::RecieveItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human* pHuman, _ITEM* pItem)
{
	Assert(pHuman);
	Assert(pItem);
	Assert(!pItem->IsNullType());
	INT newSpace = INVALID_INDEX;

	ItemContainer*		pItemContainer;
	pItemContainer = pHuman->GetBaseContain();
	Assert(pItemContainer);

	INT nFinalIndex = g_ItemOperator.MoveItem(pItemContainer, pItem);

	if(nFinalIndex<0)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pItemContainer = pHuman->GetExtraBagContain(i);
			Assert(pItemContainer);
			if (pItemContainer->IsValid())
			{
				if (pItemContainer->IsCanUse())
				{
					nFinalIndex = g_ItemOperator.MoveItem(pItemContainer, pItem);
				}
			}
			if (nFinalIndex>=0)
			{
				break;
			}
		} 
	}
	if (nFinalIndex >= 0)
	{
		Item* pDestItem = pItemContainer->GetItem(nFinalIndex);
		Assert(pDestItem);

		if(pDestItem->GetLayedNum() == pItem->GetItemCount())
		{
			GCNotifyEquip Msg;
			Msg.SetBagIndex( pItemContainer->ConIndex2BagIndex(nFinalIndex) );
			pDestItem->SaveValueTo(Msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		else
		{
			GCItemInfo  Msg;
			Msg.setIsNull(FALSE);
			Msg.setID(pItemContainer->ConIndex2BagIndex(nFinalIndex));
			pDestItem->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		pLogParam->ItemGuid		=	pDestItem->GetGUID();
		pLogParam->ItemType		=	pDestItem->GetItemTableIndex();
		pLogParam->ContainerPos		=	pItemContainer->ConIndex2BagIndex(nFinalIndex);
		pLogParam->CharGUID		=	pHuman->GetGUID();
		pLogParam->SceneID		=	pHuman->getScene()->SceneID();
		pLogParam->XPos			=	pHuman->getWorldPos()->m_fX;
		pLogParam->ZPos			=	pHuman->getWorldPos()->m_fZ;
		

		//绑定设置
		if(pDestItem->IsRuler(IRL_PICKBIND))
		{

			ItemContainer* pContainer = HumanItemLogic::GetBagContainer(pHuman,pItemContainer->ConIndex2BagIndex(nFinalIndex));
			if(pContainer)
			{
				g_ItemOperator.SetItemBind(pContainer,nFinalIndex);
			}
		}
		
		//通知脚本,修正物品改变以后没有通知脚本的bug
		pHuman->OnItemChanged( pLogParam->ItemType );

		return TRUE;
	}
	return FALSE;

}

BOOL	HumanItemLogic::EraseBagItem(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		return FALSE;
	}
	
	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
	Assert(pItem);
	pLogParam->ItemGuid		=	pItem->GetGUID();
	pLogParam->Count		=	pItem->GetLayedNum();

	BOOL bRet = g_ItemOperator.EraseItem(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex));

	if(bRet)
	{
		pLogParam->CharGUID		=	pHuman->GetGUID();
		pLogParam->SceneID		=	pHuman->getScene()->SceneID();
		pLogParam->XPos			=	pHuman->getWorldPos()->m_fX;
		pLogParam->ZPos			=	pHuman->getWorldPos()->m_fZ;
	}
	Assert(bRet);

	GCItemInfo	 Msg;
	Msg.setIsNull(TRUE);
	Msg.setID(uBagIndex);
	pHuman->GetPlayer()->SendPacket(&Msg);

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::DecBagItemLayCount(Obj_Human* pHuman,UINT uBagIndex,UINT uCount /* =1 */)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		return FALSE;
	}

	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->IsEmpty())
		return FALSE;

	if(pItem->IsLock())
		return FALSE;

	if(pItem->GetLayedNum()==0)
	{
		Assert(FALSE);
	}
	else if(pItem->GetLayedNum()>0)
	{
		BOOL bRet = g_ItemOperator.DecItemLayCount(pContainer,
			pContainer->BagIndex2ConIndex(uBagIndex),
			uCount);

		if(bRet)
		{
			Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
			if(pItem->GetLayedNum()==0)
			{
				g_ItemOperator.EraseItem(pContainer,
					pContainer->BagIndex2ConIndex(uBagIndex));

				GCItemInfo	Msg;
				Msg.setID(uBagIndex);
				Msg.setIsNull(TRUE);
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
			else
			{
				GCItemInfo	 Msg;
				Msg.setID(uBagIndex);
				Msg.setIsNull(FALSE);
				pItem->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		}
		return bRet;
	}
	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::IncBagItemLayCount(Obj_Human* pHuman,UINT uBagIndex,UINT uCount )
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		return FALSE;
	}
	
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->IsEmpty())
		return FALSE;

	if(pItem->IsLock())
		return FALSE;

	BOOL bRet = g_ItemOperator.IncItemLayCount(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		uCount);

	if(bRet)
	{
		Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

		GCItemInfo	Msg;
		Msg.setID(uBagIndex);
		Msg.setIsNull(FALSE);
		pItem->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);
	}
	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

INT		HumanItemLogic::CalcBagItemSpace(Obj_Human* pHuman,ItemContainer* pContainer, UINT uItemIndex, INT Number,INT&  NewSpace)
{
	__ENTER_FUNCTION

		Assert(Number>0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pContainer);
	Assert(pContainer->IsValid());

	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	INT		Count		= 0; // 空格子能够提供的空间
	INT		PileCount	= 0; // 已经占用了格子的同类物品可以提供的容纳空间，仅对可叠加物品有效
	INT		Tile		= 0; // 一格能够放几个同类物品

	_ITEM_TYPE	 type = ConvertSerial2ItemType(uItemIndex);
	Tile	=	GetItemTileMax(type);
	Assert(Tile>0);

	for( UINT i=0; i<(UINT)pContainer->GetContainerSize(); ++i )
	{
		Item* pItem = pContainer->GetItem(i);
		Assert(pItem);

		if( Tile == 1 )
		{
			if( pItem->IsEmpty() == TRUE )
			{
				++Count;
			}
		}
		else
		{
			if( pItem->IsEmpty() == TRUE )
			{
				Count += Tile;
			}
			else if( (pItem->GetItemTableIndex() == uItemIndex)
				&& (pItem->IsCanLay()))
			{
				PileCount += Tile - pItem->GetLayedNum();
			}
		}
	}

	if( Count + PileCount >= Number )
	{
		INT sum = Number - PileCount;
		if(sum<=0)
			NewSpace = 0;
		else
			NewSpace	=	sum / Tile + ((sum % Tile) ? 1 : 0);
	}

	return Number - Count - PileCount;

	__LEAVE_FUNCTION

		return Number;
}

BOOL	HumanItemLogic::CalcBagItemSpace(Obj_Human* pHuman,UINT uItemIndex,INT Number,INT& NewSpace)
{
	__ENTER_FUNCTION

		Assert(Number>0);
	Assert(uItemIndex!=INVALID_ID);

	ItemContainer*		pContainer;

	pContainer = pHuman->GetBaseContain();
	Assert(pContainer);

	INT nSpace = 0;
	INT nRemainCount = CalcBagItemSpace(pHuman, pContainer, uItemIndex, Number, NewSpace);

	if(nRemainCount>0)
	{
		INT nSpace = 0;
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pContainer = pHuman->GetExtraBagContain(i);
			Assert(pContainer);
			if (pContainer->IsValid())
			{
				if (pContainer->IsCanUse())
				{
					nRemainCount = CalcBagItemSpace(pHuman, pContainer, uItemIndex, nRemainCount, nSpace);
					NewSpace += nSpace;
				}
			}
			if (nRemainCount<=0)
			{
				break;
			}
		} 
	}

	if (nRemainCount<=0)
	{
		return TRUE;
	}
	__LEAVE_FUNCTION

		return FALSE;
}

INT		HumanItemLogic::CalcBagSpace(Obj_Human* pHuman)
{
	__ENTER_FUNCTION

		Assert(pHuman);
	if(!pHuman)
	{
		Assert(FALSE);
		return 0;
	}

	INT nSpace = 0;
	ItemContainer*	pContainer = NULL;
	pContainer = pHuman->GetBaseContain();
	Assert(pContainer);

	nSpace += pContainer->CalcItemSpace();
	for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
	{
		pContainer = pHuman->GetExtraBagContain(i);
		Assert(pContainer);
		if (pContainer->IsValid())
		{
			if (pContainer->IsCanUse())
			{
				nSpace += pContainer->CalcItemSpace();
			}
		}
	} 

	return nSpace;
	__LEAVE_FUNCTION

		return	0;
}


BOOL	HumanItemLogic::CanBagReceiveItemList(Obj_Human* pHuman,_ITEM_LIST& List)
{
	__ENTER_FUNCTION

	ItemContainer*  pBaseContain	= pHuman->GetBaseContain();

	Assert(pBaseContain);

	INT iBaseSize = pBaseContain->CalcItemSpace();

	for(INT iListIndex =0;iListIndex<List.m_ListCount;iListIndex++ )
	{	
		UINT uItemIndex = List.m_ListType[iListIndex].ToSerial();
		UINT uCount		= List.m_TypeCount[iListIndex];
		INT iNewSpace;
		if(HumanItemLogic::CalcBagItemSpace(pHuman,uItemIndex,uCount,iNewSpace))
		{

			UCHAR uClass =  GetSerialClass(uItemIndex);

			switch(uClass) 
			{
			case ICLASS_TASKITEM:
			case ICLASS_EQUIP:
			case ICLASS_COMITEM:
			case ICLASS_STOREMAP:
			case ICLASS_GEM:
			case ICLASS_MATERIAL:
			case ICLASS_IDENT:
				{
					iBaseSize-=iNewSpace;
					if(iBaseSize<0)
						return FALSE;
				}
				break;
			default:
				Assert(FALSE);
				return FALSE;
				break;
			}
		}
		else
			return FALSE;
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::ReceiveBagItemList(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,_ITEM_LIST& List)
{
	__ENTER_FUNCTION

		Assert(List.m_ListCount>0);

	for(INT i =0;i<List.m_ListCount;i++)
	{
		_ITEM_TYPE	itemType =List.m_ListType[i];
		if(!CreateMultiItemToBag(pLogParam,pHuman,itemType.ToSerial(),List.m_TypeCount[i]))
		{
			return FALSE;
		}
	}
	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

Item*	HumanItemLogic::GetBagItem(Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION
	Assert(pHuman);

	if(uBagIndex>EXTRA_CONTAINER_OFFSET)
		return NULL;
	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);

	if(!pContainer)
	{	
		return NULL;
	}

	return pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

	__LEAVE_FUNCTION

		return NULL;
}

INT HumanItemLogic::GetBagItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid)
{
	Assert(pHuman);
	Assert(guid.isNull()==FALSE);

	INT iFinalIndex = INVALID_ID;
	
	iFinalIndex = pHuman->GetBaseContain()->GetIndexByGUID(&guid);
	if(iFinalIndex!=INVALID_ID)
		return pHuman->GetBaseContain()->ConIndex2BagIndex(iFinalIndex);

	if (iFinalIndex == INVALID_ID)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			ItemContainer* pContainer = pHuman->GetExtraBagContain(i);
			if (pContainer->IsValid())
			{
				iFinalIndex = pContainer->GetIndexByGUID(&guid);
			}
			
			if (iFinalIndex != INVALID_ID)
			{
				return pHuman->GetExtraBagContain(i)->ConIndex2BagIndex(iFinalIndex);
			}
		}
	}

	return INVALID_ID;
}

INT	HumanItemLogic::GetBagItemPosByType(Obj_Human* pHuman,UINT uItemIndex)
{
	Assert(uItemIndex>0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pHuman);

	INT iFinalIndex = INVALID_ID;

	iFinalIndex = pHuman->GetBaseContain()->GetIndexByType(uItemIndex);
	if(iFinalIndex!=INVALID_ID)
		return pHuman->GetBaseContain()->ConIndex2BagIndex(iFinalIndex);

	if (iFinalIndex == INVALID_ID)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			ItemContainer* pContainer = pHuman->GetExtraBagContain(i);
			if (pContainer->IsValid())
			{
				iFinalIndex = pContainer->GetIndexByType(uItemIndex);
			}
			if (iFinalIndex != INVALID_ID)
			{
				return pHuman->GetExtraBagContain(i)->ConIndex2BagIndex(iFinalIndex);
			}
		}
	}

	return iFinalIndex;
}


INT	HumanItemLogic::CalcBagItemCount(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

		Assert(uItemIndex >0);
	Assert(pHuman);

	INT count = 0;
	Item* pItem = NULL;
	ItemContainer* pContainer = pHuman->GetBaseContain();
	if(!pContainer)
	{
		return 0;
	}

	for(int i=0; i<pContainer->GetContainerSize(); ++i)
	{
		pItem = pContainer->GetItem(i);

		if(!pItem)
		{
			Assert(pItem);
			continue;
		}
		if( pItem->GetItemTableIndex()	== uItemIndex )
		{
			count += pItem->GetLayedNum();
		}
	}

	for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
	{
		pContainer = pHuman->GetExtraBagContain(i);
		if (pContainer->IsValid())
		{
			for(int i=0; i<pContainer->GetContainerSize(); ++i)
			{
				pItem = pContainer->GetItem(i);

				if(!pItem)
				{
					Assert(pItem);
					continue;
				}
				if( pItem->GetItemTableIndex()	== uItemIndex )
				{
					count += pItem->GetLayedNum();
				}
			}
		}
	}

	return count;

	__LEAVE_FUNCTION

		return 0 ;
}

BOOL	HumanItemLogic::SetBagItemDur(Obj_Human* pHuman,UINT uBagIndex, INT iDur)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);
	
	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	return g_ItemOperator.SetItemDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemCurMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	
	return g_ItemOperator.SetItemCurMaxDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemIdent(Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	BOOL bRet = g_ItemOperator.SetItemIdent(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex));
	if(bRet)
	{
		Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
		Assert(pItem);
		GCItemInfo Msg;
		pItem->SaveValueTo(Msg.getItem());
		Msg.setID(uBagIndex);
		Msg.setIsNull(FALSE);
		pHuman->GetPlayer()->SendPacket(&Msg);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemParam(Obj_Human* pHuman,UINT uBagIndex,UINT Start,INT Type,INT value)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	BOOL bRet = g_ItemOperator.SetItemParam(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),Start,Type,value);

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemMaxDur(Obj_Human* pHuman,UINT uBagIndex, INT iDur)
{
	__ENTER_FUNCTION

		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}

	return g_ItemOperator.SetItemMaxDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::CreateMultiItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human* pHuman,UINT uItemIndex,UINT uCount)
{
	__ENTER_FUNCTION

		Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(uCount>0);
	Assert(pHuman);
	Assert(pLogParam);

	INT iNewCount;

	pLogParam->CharGUID		=	pHuman->GetGUID();
	pLogParam->SceneID		=	pHuman->getScene()->SceneID();
	pLogParam->XPos			=	pHuman->getWorldPos()->m_fX;
	pLogParam->ZPos			=	pHuman->getWorldPos()->m_fZ;

	if(CalcBagItemSpace(pHuman,uItemIndex,uCount,iNewCount))
	{
		UINT iBagIndex = INVALID_INDEX;
		UINT iOldBagIndex = INVALID_INDEX;
		UINT uLastCount = uCount;
		UINT uEachPileCount = 0;
		for(UINT iCount =0;iCount<uCount;iCount++)
		{
			BOOL ret = CreateItemToBag(pLogParam,pHuman,uItemIndex,iBagIndex);
			
			if (!ret)
			{
				return FALSE;
			}
			if(iCount ==0) 
			{
				iOldBagIndex = iBagIndex;
			}

			if(iBagIndex!=iOldBagIndex)
			{
				GCNotifyEquip	 Msg;
				Msg.SetBagIndex( iOldBagIndex );
				Item* pItem = GetBagItem(pHuman,iOldBagIndex);
				Assert(pItem);
				pItem->SaveValueTo(Msg.GetItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
				iOldBagIndex =	iBagIndex;

				//最后一堆的个数
				uLastCount -= uEachPileCount;
				uEachPileCount = 1;
			}
			else
			{
				uEachPileCount++;
			}
		}

		if(iBagIndex!=INVALID_INDEX)
		{
			Item* pChangedItem = GetBagItem(pHuman, iBagIndex);

			Assert(pChangedItem);

			if(pChangedItem->GetLayedNum()==uLastCount)
			{
				GCNotifyEquip Msg;
				Msg.SetBagIndex( iBagIndex );
				Item* pItem = GetBagItem(pHuman,iBagIndex);
				Assert(pItem);
				pItem->SaveValueTo(Msg.GetItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
			else
			{
				GCItemInfo  Msg;
				Msg.setIsNull(FALSE);
				Msg.setID(iBagIndex);
				pChangedItem->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
		}
		return TRUE;
	}
	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	HumanItemLogic::CreateItemToBag(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,UINT uItemIndex,UINT& uBagIndex,UINT uQuality )
{
	__ENTER_FUNCTION

		Assert(pHuman);
	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pLogParam);

	if(uItemIndex==0||uItemIndex==INVALID_ID)
		return FALSE;

	ItemContainer*		pContainer;
	BOOL bNewPos;
	UINT uContainerPos;

	pContainer = pHuman->GetBaseContain();
	Assert(pContainer);

	BOOL	bRet =  g_ItemOperator.CreateItem(pLogParam,uItemIndex,uQuality,pContainer,bNewPos,uContainerPos);

	if(!bRet)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pContainer = pHuman->GetExtraBagContain(i);
			Assert(pContainer);
			if(pContainer->IsValid())
			{
				if (pContainer->IsCanUse())
				{
					bRet =  g_ItemOperator.CreateItem(pLogParam,uItemIndex,uQuality,pContainer,bNewPos,uContainerPos);
				}
			}
			if (bRet)
			{
				break;
			}
		} 
	}

	if(bRet)
	{
		pHuman->OnItemChanged( uItemIndex ); //通知脚本
		uBagIndex = pContainer->ConIndex2BagIndex(uContainerPos);
		//绑定处理
		Item* pItem = pContainer->GetItem(uContainerPos);
		Assert(pItem);
		if(pItem->IsRuler(IRL_PICKBIND))
		{
			g_ItemOperator.SetItemBind(pContainer,uContainerPos);
		}
	}
	else
	{
		uBagIndex = INVALID_INDEX;
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	HumanItemLogic::CreateItemToBag(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman, 
										_CREATEITEM_INIT* pInit, UINT& uBagIndex,UINT uQuality )
{
	__ENTER_FUNCTION

		Assert(pHuman);
	Assert(pLogParam);

	if(pInit->m_nItemIndex==0||pInit->m_nItemIndex==INVALID_ID)
		return FALSE;

	ItemContainer*	pContainer = pHuman->GetBaseContain();

	Assert(pContainer);

	if(!pContainer)
	{
		return FALSE;
	}

	UINT uContainerPos;

	BOOL	bRet =  g_ItemOperator.CreateItem(pLogParam,uQuality,  pContainer, uContainerPos, pInit);

	if(bRet)
	{
		pHuman->OnItemChanged( pInit->m_nItemIndex ); //通知脚本
		uBagIndex = pContainer->ConIndex2BagIndex(uContainerPos);
		//绑定处理
		Item* pItem = pContainer->GetItem(uContainerPos);
		Assert(pItem);
		if(pItem->IsRuler(IRL_PICKBIND))
		{
			g_ItemOperator.SetItemBind(pContainer,uContainerPos);
		}
	}
	else
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pContainer = pHuman->GetExtraBagContain(i);
			Assert(pContainer);
			if(pContainer->IsValid())
			{
				if (pContainer->IsCanUse())
				{
					bRet =  g_ItemOperator.CreateItem(pLogParam,uQuality, pContainer, uContainerPos, pInit);
				}
			}
			
			if(bRet)
			{
				pHuman->OnItemChanged( pInit->m_nItemIndex ); //通知脚本
				uBagIndex = pContainer->ConIndex2BagIndex(uContainerPos);
				//绑定处理
				Item* pItem = pContainer->GetItem(uContainerPos);
				Assert(pItem);
				if(pItem->IsRuler(IRL_PICKBIND))
				{
					g_ItemOperator.SetItemBind(pContainer,uContainerPos);
				}
				break;
			}
		} 
		if(!bRet)
			uBagIndex = INVALID_INDEX;
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

//锁定背包内的物品
VOID	HumanItemLogic::LockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID)
{
	__ENTER_FUNCTION
		Assert(pHuman);
	Assert(pItemID);

	ItemContainer*	pMyBagContainer = GetBagItemContain(pHuman, pItemID);
	Assert(pMyBagContainer);

	INT	BagIndex = pMyBagContainer->GetIndexByGUID(pItemID);
	if(BagIndex != INVALID_INDEX)
		g_ItemOperator.LockItem( pMyBagContainer, BagIndex );

	__LEAVE_FUNCTION
}

//解锁背包内的物品
VOID	HumanItemLogic::UnlockBagItem(Obj_Human*	pHuman, _ITEM_GUID* pItemID )
{
	__ENTER_FUNCTION
		Assert(pHuman);
	Assert(pItemID);

	ItemContainer*	pMyBagContainer = GetBagItemContain(pHuman, pItemID);
	Assert(pMyBagContainer);

	INT	BagIndex = pMyBagContainer->GetIndexByGUID(pItemID);
	if(BagIndex != INVALID_INDEX)
		g_ItemOperator.UnlockItem( pMyBagContainer, BagIndex );

	__LEAVE_FUNCTION
}

BOOL	HumanItemLogic::SetBagEquipDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur)
{
	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	return g_ItemOperator.SetItemDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);
}

BOOL	HumanItemLogic::SetBagEquipMaxDur(Obj_Human*	pHuman, UINT uBagIndex, INT iDur)
{
	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	return g_ItemOperator.SetItemMaxDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);
}

BOOL	HumanItemLogic::SetCurBagEquipMaxDur(Obj_Human*	pHuman,UINT uBagIndex, INT iDur)
{
	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetBagContainer(pHuman,uBagIndex);
	if (pContainer == NULL)
	{
		return FALSE;
	}
	if (!pContainer->IsCanUse())
	{
		return FALSE;
	}
	return g_ItemOperator.SetItemCurMaxDur(pContainer,
		pContainer->BagIndex2ConIndex(uBagIndex),
		iDur);
}

BOOL	HumanItemLogic::RecieveItemToBag(Obj_Human* pHuman, ItemContainer* pContainer, UINT uIndex )
{
	__ENTER_FUNCTION

		Assert(pHuman);
	Assert(pContainer->IsValid());
	Assert(uIndex<(UINT)pContainer->GetContainerSize());
	Item*	pItemInSource = pContainer->GetItem(uIndex);
	if(pItemInSource->IsEmpty())
	{
		return FALSE;
	}
	UINT uItemIndex = pItemInSource->GetItemTableIndex();
	UINT uCount = pItemInSource->GetLayedNum();

	BOOL	bFlag = FALSE;
	g_ItemOperator.UnlockItem( pContainer, uIndex );

	ItemContainer*		pMyContainer = pHuman->GetBaseContain();
	Assert(pMyContainer);
	//自动找格,支持自动叠加
	INT result = 
		g_ItemOperator.MoveItem
		(
		pContainer,
		uIndex,
		bFlag,
		pMyContainer
		);

	if(result<0)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pMyContainer = pHuman->GetExtraBagContain(i);
			Assert(pMyContainer);
			if(pMyContainer->IsValid())
			{
				if (pMyContainer->IsCanUse())
				{
					result = 
						g_ItemOperator.MoveItem
						(
						pContainer,
						uIndex,
						bFlag,
						pMyContainer
						);
				}
			}
			
			if (result>=0)
			{
				break;
			}
		}
		if (result<0)
		{
			return FALSE;
		}
	}

	Item*	pItemDest = pMyContainer->GetItem(result);
	UINT	ToIndex = pMyContainer->ConIndex2BagIndex(result);
	if(pItemDest->IsRuler(IRL_PICKBIND))
	{
		g_ItemOperator.SetItemBind(pMyContainer,result);
	}

	if(pItemDest->GetLayedNum() == uCount)
	{
		GCNotifyEquip Msg;
		Msg.SetBagIndex( ToIndex );
		pItemDest->SaveValueTo(Msg.GetItem());
		pHuman->GetPlayer()->SendPacket(&Msg);
	}
	else
	{
		GCItemInfo  Msg;
		Msg.setIsNull(FALSE);
		Msg.setID(ToIndex);
		pItemDest->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);
	}

	return TRUE;
	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::CanReceiveExchangeItemList(Obj_Human* pHuman,_EXCHANGE_ITEM_LIST& List)
{
	__ENTER_FUNCTION

		ItemContainer*  pBaseContain	= pHuman->GetBaseContain();
	ItemContainer*	pPetContain		= pHuman->GetPetContain();

	Assert(pBaseContain);
	Assert(pPetContain);

	ItemContainer*	pMyContainer = NULL;
	INT iBaseSize = pBaseContain->CalcExchangeItemSpace();
	INT iPetSize =  pPetContain->CalcExchangeItemSpace();
	for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
	{
		pMyContainer = pHuman->GetExtraBagContain(i);
		if (pMyContainer->IsValid())
		{
			iBaseSize += pMyContainer->CalcExchangeItemSpace();
		}
	}

	for(INT iListIndex =0;iListIndex<List.m_ListCount;iListIndex++ )
	{	
		Item* pItem = List.m_ListItem[iListIndex];
		if(pItem->GetDataType() == IDT_PET)
		{
			iPetSize-=1;
			if(iPetSize<0)
				return FALSE;
		}
		else if(pItem->GetDataType() == IDT_ITEM)
		{
			switch(pItem->GetItemClass()) 
			{
			case ICLASS_TASKITEM:
			case ICLASS_EQUIP:
			case ICLASS_COMITEM:
			case ICLASS_STOREMAP:
			case ICLASS_GEM:
			case ICLASS_MATERIAL:
			case ICLASS_IDENT:
				{
					iBaseSize-=1;
					if(iBaseSize<0)
						return FALSE;
				}
				break;
			default:
				Assert(FALSE);
				return FALSE;
				break;
			}
		}
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::ExchangeBagItem(Obj_Human*	pHuman1,UINT uBagIndex1, Obj_Human* pHuman2, UINT uBagIndex2)
{
	__ENTER_FUNCTION

	Assert(pHuman1);
	Assert(pHuman2);

	ItemContainer* pContainer1 = GetBagContainer(pHuman1, uBagIndex1);
	ItemContainer* pContainer2 = GetBagContainer(pHuman2, uBagIndex2);

	if ((pContainer1==NULL)||(pContainer2==NULL))
	{
		return FALSE;
	}

	if ((!pContainer1->IsCanUse())||(!pContainer2->IsCanUse()))
	{
		return FALSE;
	}

	INT Ret = g_ItemOperator.ExchangeItem(pContainer1, uBagIndex1, pContainer2, uBagIndex2);
	if (Ret>=0)
	{
		return TRUE;
	}

	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}