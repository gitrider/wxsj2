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

ItemContainer*	HumanItemLogic::GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID, INT& nBagIndex)
{
	__ENTER_FUNCTION
		Assert(pHuman);
	Assert(pItemID);

	ItemContainer* pItemContainer = pHuman->GetBaseContain();
	INT FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
	if (FinalIndex == INVALID_INDEX)
	{
		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA1);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				nBagIndex = pItemContainer->ConIndex2BagIndex(FinalIndex);
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA2);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				nBagIndex = pItemContainer->ConIndex2BagIndex(FinalIndex);
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA3);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				nBagIndex = pItemContainer->ConIndex2BagIndex(FinalIndex);
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA4);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				nBagIndex = pItemContainer->ConIndex2BagIndex(FinalIndex);
				return pItemContainer;
			}
		}
	}
	if (FinalIndex >= 0)
	{
		nBagIndex = pItemContainer->ConIndex2BagIndex(FinalIndex);
		return pItemContainer;
	}
	
	return NULL;
	__LEAVE_FUNCTION
		return NULL;
}

ItemContainer*	HumanItemLogic::GetBagItemContain(Obj_Human* pHuman, _ITEM_GUID* pItemID)
{
	__ENTER_FUNCTION
		Assert(pHuman);
	Assert(pItemID);

	ItemContainer* pItemContainer = pHuman->GetBaseContain();
	INT FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
	if (FinalIndex == INVALID_INDEX)
	{
		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA1);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA2);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA3);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				return pItemContainer;
			}
		}

		pItemContainer = pHuman->GetExtraBagContain(BAG_EXTRA4);
		if (pItemContainer->IsValid())
		{
			FinalIndex = pItemContainer->GetIndexByGUID(pItemID);
			if (FinalIndex != INVALID_INDEX)
			{
				return pItemContainer;
			}
		}
	}
	
	if (FinalIndex<0)
	{
		return NULL;
	}

	return pItemContainer;

	__LEAVE_FUNCTION
		return NULL;
}

ItemContainer*	HumanItemLogic::GetBagContainer(Obj_Human*	pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(pHuman);
		Assert(uBagIndex<MAX_BAG_SIZE);

		ItemContainer* pContainer = NULL;

		if(uBagIndex<EXTRA_BAG1_OFFSET)
			return pHuman->GetBaseContain();
		else if (uBagIndex<EXTRA_BAG2_OFFSET)
		{
			pContainer = pHuman->GetExtraBagContain(BAG_EXTRA1);
			if (pContainer->IsValid())
			{
				if (pContainer->IsInContainer(uBagIndex))
				{
					return pContainer;
				}
			}
			
			return NULL;
		}
		else if (uBagIndex<EXTRA_BAG3_OFFSET)
		{
			pContainer = pHuman->GetExtraBagContain(BAG_EXTRA2);
			if (pContainer->IsValid())
			{
				if (pContainer->IsInContainer(uBagIndex))
				{
					return pContainer;
				}
			}
			return NULL;
		}
		else if (uBagIndex<EXTRA_BAG4_OFFSET)
		{
			pContainer = pHuman->GetExtraBagContain(BAG_EXTRA3);
			if (pContainer->IsValid())
			{
				if (pContainer->IsInContainer(uBagIndex))
				{
					return pContainer;
				}
			}
			return NULL;
		}
		else if(uBagIndex<EXTRA_CONTAINER_OFFSET)
		{
			pContainer = pHuman->GetExtraBagContain(BAG_EXTRA4);
			if (pContainer->IsValid())
			{
				if (pContainer->IsInContainer(uBagIndex))
				{
					return pContainer;
				}
			}
			return NULL;
		}

		return NULL;
	__LEAVE_FUNCTION

		return NULL;
}

ItemContainer*	HumanItemLogic::GetMountContainer(Obj_Human*	pHuman)  // [7/8/2010 陈军龙]
{
	__ENTER_FUNCTION

	Assert(pHuman);

	return pHuman->GetMountContain();

	__LEAVE_FUNCTION

	return NULL;
}

Item*	HumanItemLogic::GetMount(Obj_Human* pHuman, INT nIndex)          // [7/8/2010 陈军龙]
{
	__ENTER_FUNCTION

	Assert(nIndex < MAX_MOUNT_SIZE);
	Assert(nIndex >= 0);

	if(nIndex < 0 || nIndex > MAX_MOUNT_SIZE)
	{
		AssertEx(FALSE,"HumanItemLogic::GetMount() GetMount 不在合法范围 ");
		return NULL;
	}

	ItemContainer*	pMountContainer = pHuman->GetMountContain();

	Assert(pMountContainer);

	if(!pMountContainer)
	{
		return NULL;
	}

	return pMountContainer->GetItem((UINT)nIndex);

	__LEAVE_FUNCTION

	return NULL;
}

BOOL	HumanItemLogic::MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,ItemContainer* pContainer,UINT uIndex, INT& uBagPos)
{
	__ENTER_FUNCTION
		
		Assert(pLogParam);
		Assert(pHuman);
		Assert(pContainer);
		Assert(pContainer->IsValid());
		Assert(uIndex<(UINT)pContainer->GetContainerSize());
		
		//得到源物品数据
		Item*	pSourItem = pContainer->GetItem(uIndex);
		Assert(pSourItem);
		
		if(!pSourItem)
		{
			return FALSE;
		}

		pLogParam->ItemGuid = pSourItem->GetGUID();
		pLogParam->Count	= pSourItem->GetLayedNum();
		uBagPos = INVALID_INDEX;

		UINT uItemIndex = pSourItem->GetItemTableIndex();
		
		pLogParam->ItemType	=	uItemIndex;

		//获得目的包位置
		INT uContainIndex = -1;
		//判断放置的背包位置
		ItemContainer*	pHumanContainer = pHuman->GetBaseContain();
		Assert(pHumanContainer);

		//获得目的包位置
		uContainIndex = g_ItemOperator.MoveItem(pContainer,uIndex,pHumanContainer);
		if (uContainIndex<0)
		{
			for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
			{
				pHumanContainer = pHuman->GetExtraBagContain(i);
				Assert(pHumanContainer);
				if (pHumanContainer->IsValid())
				{
					if (pHumanContainer->IsCanUse())
					{
						//获得目的包位置
						uContainIndex = g_ItemOperator.MoveItem(pContainer,uIndex,pHumanContainer);
						if (uContainIndex>=0)
						{
							break;
						}
					}
				}
			}
		}

		if (uContainIndex<0)
		{
			return FALSE;
		}
		else if(uContainIndex<(UINT)pHumanContainer->GetContainerSize())
		{
			//转换成BagIndex
			uBagPos = pHumanContainer->ConIndex2BagIndex(uContainIndex);
			
			
			pHuman->OnItemChanged( uItemIndex); //通知脚本
			return TRUE; //成功
		}

		return FALSE;
			
	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::MoveItem(ITEM_LOG_PARAM* pLogParam,
								 Obj_Human*	pHuman,
								 ItemContainer*	pContainer,
								 UINT uIndex,
								 Obj_Human* pDestMan,
								 INT& uBagPos)
{
	__ENTER_FUNCTION

		Assert(pLogParam);
	Assert(pHuman);
	Assert(pContainer);
	Assert(pContainer->IsValid());
	Assert(pDestMan);
	Assert(uIndex<(UINT)pContainer->GetContainerSize());

	//得到源物品数据
	Item*	pSourItem = pContainer->GetItem(uIndex);
	Assert(pSourItem);

	if(!pSourItem)
	{
		return FALSE;
	}

	pLogParam->ItemGuid = pSourItem->GetGUID();
	pLogParam->Count	= pSourItem->GetLayedNum();
	uBagPos = INVALID_INDEX;

	UINT uItemIndex = pSourItem->GetItemTableIndex();

	pLogParam->ItemType	=	uItemIndex;

	//获得目的包位置
	INT uContainIndex = -1;
	//判断放置的背包位置
	ItemContainer*	pHumanContainer = pDestMan->GetBaseContain();
	Assert(pHumanContainer);

	//获得目的包位置
	uContainIndex = g_ItemOperator.MoveItem(pContainer,uIndex,pHumanContainer);
	if (uContainIndex<0)
	{
		for (INT i=0; i<MAX_EXTRA_BAG_NUM; ++i)
		{
			pHumanContainer = pDestMan->GetExtraBagContain(i);
			Assert(pHumanContainer);
			if (pHumanContainer->IsValid())
			{
				if (pHumanContainer->IsCanUse())
				{
					//获得目的包位置
					uContainIndex = g_ItemOperator.MoveItem(pContainer,uIndex,pHumanContainer);
					if (uContainIndex>=0)
					{
						break;
					}
				}
			}
		}
	}
	
	if (uContainIndex<0)
	{
		return FALSE;
	}
	else if(uContainIndex<(UINT)pHumanContainer->GetContainerSize())
	{
		//转换成BagIndex
		uBagPos = pHumanContainer->ConIndex2BagIndex(uContainIndex);


		pHuman->OnItemChanged( uItemIndex); //通知脚本
		return TRUE; //成功
	}

	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SplitItem(Obj_Human* pHuman,
								  ItemContainer* pSourContainer, 
								  UCHAR uSourIndex, 
								  UCHAR uCount, 
								  ItemContainer* pDestContainer, 
								  INT nDestIndex,
                                  INT opType)
{
	__ENTER_FUNCTION

	Assert(pSourContainer);
	Assert(pDestContainer);
	Assert(pSourContainer->IsValid());
	Assert(uSourIndex<pSourContainer->GetContainerSize());

	if (!pDestContainer->IsValid())
	{
		Assert(FALSE);
		return FALSE;
	}
	Item* pSourceItem = pSourContainer->GetItem(uSourIndex);

	if(!pSourceItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pSourceItem->IsEmpty())
	{
		return FALSE;
	}

	Item* pOldItem = NULL;
	BOOL  bOldEmpty = TRUE;
	if (nDestIndex != INVALID_INDEX)
	{
		pOldItem = pDestContainer->GetItem(nDestIndex);
		Assert(pOldItem);
		if (!pOldItem->IsEmpty())
		{
			bOldEmpty = FALSE;
		}
	}
	
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType		= ITEM_CREATE_SPLIT;
	ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
	ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
	ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;
	
	if(g_ItemOperator.SplitItem(&ItemLogParam,pSourContainer,uSourIndex,uCount,pDestContainer,nDestIndex)
		==ITEMOE_SUCCESS)
	{
		pSourceItem = pSourContainer->GetItem(uSourIndex);

		GCItemInfo  MsgItem;
		MsgItem.setIsNull(FALSE);
		MsgItem.setID((WORD)uSourIndex);
		pSourceItem->SaveValueTo(MsgItem.getItem());
		pHuman->GetPlayer()->SendPacket(&MsgItem);

		Item* pDestItem = pDestContainer->GetItem(nDestIndex);
		if (bOldEmpty)
		{
			GCNotifyEquip Msg;
			Msg.SetBagIndex(nDestIndex);
            Msg.SetOpType(opType);
			pDestItem->SaveValueTo(Msg.GetItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		else
		{
			GCItemInfo Msg;
			MsgItem.setIsNull(FALSE);
			MsgItem.setID((WORD)nDestIndex);
            Msg.setOpType(opType);
			pDestItem->SaveValueTo(MsgItem.getItem());
			pHuman->GetPlayer()->SendPacket(&MsgItem);
		}
		SaveItemLog(&ItemLogParam);
		return TRUE;
	}

	
	__LEAVE_FUNCTION

		return FALSE;
}

	//在容器之间合并物品
BOOL	HumanItemLogic::SpliceItem(Obj_Human* pHuman,
									   ItemContainer* pSourContainer, 
									   UCHAR uSourIndex,
									   ItemContainer* pDestContainer,
									   UCHAR uDestIndex)
{
	__ENTER_FUNCTION
	Assert(pSourContainer);
	Assert(pDestContainer);
	Assert(pSourContainer->IsValid());
	Assert(uSourIndex<pSourContainer->GetContainerSize());

	if (!pDestContainer->IsValid())
	{
		Assert(FALSE);
		return FALSE;
	}
	Item* pSourceItem	= pSourContainer->GetItem(uSourIndex);
	Item* pSestItem		= pDestContainer->GetItem(uDestIndex);

	if(!pSourceItem || !pSestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pSourceItem->IsEmpty() || pSestItem->IsEmpty())
	{
		return FALSE;
	}

	if(g_ItemOperator.SpliceItem(pSourContainer,uSourIndex,pDestContainer,uDestIndex)
		==ITEMOE_SUCCESS)
	{
		return TRUE;
	}

	__LEAVE_FUNCTION
		return FALSE;
}
