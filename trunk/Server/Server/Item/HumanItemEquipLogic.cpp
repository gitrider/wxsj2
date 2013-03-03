/********************************************************************
创建日期:	2008年3月17日
文件名称:	HumanItemEquipLogic.cpp
创建人:		李珣

文件功能： 装备栏相关逻辑操作
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

BOOL	HumanItemLogic::EraseEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint)
{
	__ENTER_FUNCTION

		Assert(pHuman);
	Assert(EquipPoint<HEQUIP_NUMBER&&EquipPoint>=0);

	ItemContainer* pContainer =  pHuman->GetEquipContain();

	Assert(pContainer);

	return g_ItemOperator.EraseItem(pContainer,(UINT)EquipPoint);

	__LEAVE_FUNCTION

		return FALSE;
}

Item*	HumanItemLogic::GetEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint)
{
	__ENTER_FUNCTION

		Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
		return NULL;
	}

	ItemContainer*	pEquipContainer = pHuman->GetEquipContain();

	Assert(pEquipContainer);

	if(!pEquipContainer)
	{
		return NULL;
	}

	return pEquipContainer->GetItem((UINT)EquipPoint);

	__LEAVE_FUNCTION

		return NULL;
}

INT		HumanItemLogic::CalcEquipItemCount(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

		Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);

	ItemContainer*	pEquipContainer = pHuman->GetEquipContain();

	Assert(pEquipContainer);

	if(!pEquipContainer)
	{
		return 0;
	}

	INT nCount = 0;

	for(int i =0;i<HEQUIP_NUMBER;i++)
	{
		Item* pEquipItem =  pEquipContainer->GetItem(i);

		if(!pEquipItem)
			continue;

		if(pEquipItem->GetItemTableIndex()==uItemIndex)
		{
			if(pEquipItem->GetLayedNum()>0)
			{
				nCount+=pEquipItem->GetLayedNum();
			}
		}
	}

	return nCount;

	__LEAVE_FUNCTION

		return 0;
}

BOOL	HumanItemLogic::SetEquipDamagePoint(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, UINT nDamagePoint)
{
	Assert(pHuman);
	Assert(EquipPoint<HEQUIP_NUMBER);

	ItemContainer* pEquipContain = pHuman->GetEquipContain();
	Assert(pEquipContain);

	return g_ItemOperator.SetItemDamagePoint(pEquipContain, EquipPoint, nDamagePoint);
}

BOOL	HumanItemLogic::SetEquipDur(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, INT iDur)
{
	__ENTER_FUNCTION

		Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
		return FALSE;
	}

	return g_ItemOperator.SetItemDur(pHuman->GetEquipContain(),
		EquipPoint,
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetEquipMaxDur(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, INT iDur)
{
	__ENTER_FUNCTION

		Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
		return FALSE;
	}

	return g_ItemOperator.SetItemMaxDur(pHuman->GetEquipContain(),
		EquipPoint,
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetCurEquipMaxDur(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, INT iDur)
{
	__ENTER_FUNCTION

		Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetCurEquipMaxDur() EquipPoint 不在合法范围 ");
		return FALSE;
	}

	return g_ItemOperator.SetItemCurMaxDur(pHuman->GetEquipContain(),
		EquipPoint,
		iDur);

	__LEAVE_FUNCTION

		return FALSE;
}
