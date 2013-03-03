/********************************************************************
��������:	2008��3��17��
�ļ�����:	HumanItemExtraLogic.cpp
������:		�

�ļ����ܣ� ������������߼�����
�޸ļ�¼��

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

ItemContainer*   HumanItemLogic::GetBagByExtraContainPos(Obj_Human*	pHuman,UINT uExtraIndex)
{
	__ENTER_FUNCTION

		Assert(pHuman);
	if(!pHuman)
	{
		Assert(FALSE);
		return 0;
	}
	if (uExtraIndex<EXTRA_CONTAINER_OFFSET)
	{
		return NULL;
	}
	if (uExtraIndex>MAX_BAG_SIZE)
	{
		return NULL;
	}
	ItemContainer*	pContainer = pHuman->GetExtraContain();
	Assert(pContainer);
	INT nConIndex = pContainer->BagIndex2ConIndex(uExtraIndex);
	
	return pHuman->GetExtraBagContain(nConIndex);

	__LEAVE_FUNCTION

		return	NULL;
}
/*
*	������������
*/
//��Human����ɾ������������uBagIndex λ���ϵ���Ʒ
BOOL	HumanItemLogic::EraseExtraContainerItem(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman, UINT uBagIndex)
{
	__ENTER_FUNCTION

	Assert(uBagIndex<MAX_EXTRA_BAG_NUM);
	Assert(pHuman);

	ItemContainer* pContainer = pHuman->GetExtraContain();
	if(!pContainer)
	{
		Assert(FALSE);
		return FALSE;
	}

	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
	Assert(pItem);

	if (pItem->IsEmpty())
	{
		return FALSE;
	}
	
	if (!pContainer->IsEmpty())
	{
		return FALSE;
	}

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

	pContainer->ChangeSize(0);

	GCItemInfo	 Msg;
	Msg.setIsNull(TRUE);
	Msg.setID(uBagIndex);
	pHuman->GetPlayer()->SendPacket(&Msg);

	return TRUE;
	__LEAVE_FUNCTION

		return FALSE;
}

//���㱳���ո��ܺ�
UINT	HumanItemLogic::CalcExtraContainerSpace(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

		Assert(pHuman);
	if(!pHuman)
	{
		Assert(FALSE);
		return 0;
	}

	ItemContainer*	pContainer = pHuman->GetExtraContain();
	Assert(pContainer);

	if(!pContainer)
	{
		Assert(FALSE);
		return 0;
	}

	return pContainer->CalcItemSpace();

	__LEAVE_FUNCTION

		return	0;
}

//���Human����λ��ΪuIndex��Item
Item*	HumanItemLogic::GetExtraContainerItem(Obj_Human* pHuman, UINT uExtraContainerIndex)
{
	__ENTER_FUNCTION

		Assert(uExtraContainerIndex>=EXTRA_CONTAINER_OFFSET);
	Assert(pHuman);

	ItemContainer* pContainer = pHuman->GetExtraContain();
	if(!pContainer)
	{	
		Assert(FALSE);
		return NULL;
	}

	return pContainer->GetItem(pContainer->BagIndex2ConIndex(uExtraContainerIndex));

	__LEAVE_FUNCTION

		return NULL;
}

//���Human����λ��ΪuIndex��Item
Item*	HumanItemLogic::GetExtraBagFromBagIndex(Obj_Human* pHuman, UINT uBagIndex)
{
	__ENTER_FUNCTION

	Assert((uBagIndex>=EXTRA_BAG1_OFFSET)&&(uBagIndex<EXTRA_CONTAINER_OFFSET));
	Assert(pHuman);
	Item* pItem = NULL;
	ItemContainer* pContainer = pHuman->GetExtraContain();
	Assert(pContainer);

	if (uBagIndex<EXTRA_BAG2_OFFSET)
	{
		pItem = pContainer->GetItem(BAG_EXTRA1);
		if (!pItem->IsEmpty())
		{
			Assert(pItem->GetItemClass() == ICLASS_EXTRABAG);
			return pItem;
		}
	}
	else if (uBagIndex<EXTRA_BAG3_OFFSET)
	{
		pItem = pContainer->GetItem(BAG_EXTRA2);
		if (!pItem->IsEmpty())
		{
			Assert(pItem->GetItemClass() == ICLASS_EXTRABAG);
			return pItem;
		}
	}
	else if (uBagIndex<EXTRA_BAG4_OFFSET)
	{
		pItem = pContainer->GetItem(BAG_EXTRA3);
		if (!pItem->IsEmpty())
		{
			Assert(pItem->GetItemClass() == ICLASS_EXTRABAG);
			return pItem;
		}
	}
	else if(uBagIndex<EXTRA_CONTAINER_OFFSET)
	{
		pItem = pContainer->GetItem(BAG_EXTRA4);
		if (!pItem->IsEmpty())
		{
			Assert(pItem->GetItemClass() == ICLASS_EXTRABAG);
			return pItem;
		}
	}

	Assert(FALSE);
	return NULL;

	__LEAVE_FUNCTION

		return NULL;
}

//���Human�����ж�ӦGUID����Ʒλ��
UINT	HumanItemLogic::GetExtraContainerItemPosByGUID(Obj_Human* pHuman,_ITEM_GUID guid)
{
	Assert(pHuman);
	Assert(guid.isNull()==FALSE);

	INT iFinalIndex = INVALID_ID;

	iFinalIndex = pHuman->GetExtraContain()->GetIndexByGUID(&guid);

	if(iFinalIndex!=INVALID_ID)
		return pHuman->GetExtraContain()->ConIndex2BagIndex(iFinalIndex);

	return INVALID_ID;
}
