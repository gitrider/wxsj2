#include "stdafx.h"
#include "HumanItemExtraContainer.h"
#include "HumanDB.h"

BOOL HumanItemExtraContainer::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

		if(!ItemContainer::Init(pInit))
			return FALSE;

	const _HUMAN_ITEMEXTRACONTAINER_INIT*	pHICInit = 
		static_cast<const _HUMAN_ITEMEXTRACONTAINER_INIT*>(pInit);

	if(pHICInit==NULL)
		return FALSE;

	Assert(pHICInit->m_pHumanDB);

	m_ICTType	=	pHICInit->m_ICTType;
	m_pHumanDB	=	pHICInit->m_pHumanDB;
	m_DBOffSet	=	pHICInit->m_DBOffSet;
	return TRUE;

	__LEAVE_FUNCTION

		return	FALSE;
}

UINT	HumanItemExtraContainer::ConIndex2BagIndex(UINT uConIndex)
{
	__ENTER_FUNCTION

		Assert(uConIndex!=INVALID_INDEX);

	return m_DBOffSet+uConIndex;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}

UINT	HumanItemExtraContainer::BagIndex2ConIndex(UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex>=m_DBOffSet);
	Assert(uBagIndex<m_DBOffSet+GetContainerSize());

	return	uBagIndex-m_DBOffSet;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}

BOOL	HumanItemExtraContainer::IsInContainer(UINT uBagIndex)
{
	__ENTER_FUNCTION
		if(uBagIndex >= 0 && uBagIndex< (UINT)GetContainerSize() )
			return TRUE;
		else
			return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

INT HumanItemExtraContainer::GetContainerType()
{
	__ENTER_FUNCTION

		return m_ICTType;

	__LEAVE_FUNCTION

		return -1 ;
}

//-----------------------------------------------------------------------------------------
/*
*	物品 使用方法
*/
BOOL	HumanItemExtraContainer::SetItem(const INT nIndex, const Item* pItem )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	Assert(nIndex<(INT)m_DBOffSet+m_nContainerSize);
	Assert(pItem->GetItemClass() == ICLASS_EXTRABAG);
	BOOL	bRet = ItemContainer::SetItem(nIndex,pItem);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return	bRet;
	__LEAVE_FUNCTION

		return  FALSE;
}

//设置物品重叠数量
BOOL	HumanItemExtraContainer::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION
	Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}
//减少物品重叠数量
BOOL	HumanItemExtraContainer::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}
//增加物品重叠数量
BOOL	HumanItemExtraContainer::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//删除物品
BOOL	HumanItemExtraContainer::EraseItem(UINT uIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL	bRet	=	ItemContainer::EraseItem(uIndex);

	if(bRet)
	{
		SetDBDirty(uIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

//设置物品耐久度
BOOL	HumanItemExtraContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//设置物品损伤点
BOOL		HumanItemExtraContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}


//设置物品最大耐久度
BOOL	HumanItemExtraContainer::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL	HumanItemExtraContainer::SetItemCurMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL		HumanItemExtraContainer::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	Assert(pItem);
	Assert(nIndex<(INT)m_DBOffSet+m_nContainerSize);
	Assert(pItem->ItemClass() == ICLASS_EXTRABAG);

	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pItem);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL		HumanItemExtraContainer::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	Assert(nIndex<(INT)m_DBOffSet+m_nContainerSize);

	BOOL bRet = ItemContainer::SetItemBind(nIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanItemExtraContainer::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL	HumanItemExtraContainer::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION
	Assert(nIndex<(INT)m_DBOffSet+m_nContainerSize);
	Assert(m_pHumanDB);

	BOOL bRet = ItemContainer::SetItemCreator(nIndex,CreatorName);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


//增加物品属性
BOOL		HumanItemExtraContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//删除物品属性
BOOL		HumanItemExtraContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//删除宝石信息
BOOL		HumanItemExtraContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//添加宝石信息
BOOL		HumanItemExtraContainer::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//-----------------------------------------------------------------------------------------
/*
*	宠物 使用方法
*/
INT		HumanItemExtraContainer::GetIndexByGUID( const PET_GUID_t* pGuid )
{
	__ENTER_FUNCTION
		Assert(0);
	return INVALID_INDEX;
	__LEAVE_FUNCTION
		return INVALID_INDEX;
}

BOOL HumanItemExtraContainer::SetItemGuid(INT nIndex,PET_GUID_t* pGUID)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemExtraContainer::SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID) 				//设置宠物GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID) 				//设置宠物GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetDataID(INT nIndex,INT ID)							//设置宠物模型		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetName(INT nIndex,const CHAR* pName)						//设置名字
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetNick(INT nIndex,const CHAR* pNick)						//设置昵称
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetLevel(INT nIndex,INT level)							//设置等级
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//设置携带等级
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetAttackType(INT nIndex,INT attackType)				//设置进攻类型（物/法）
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetAIType(INT nIndex,INT AIType)						//设置AI类型
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//设置阵营
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetPetType(INT nIndex,BYTE PetType)					//宝宝，变异，野生
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetGeneration(INT nIndex,BYTE Gen)						//几代宠
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//快乐度		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetStrengthPer(INT nIndex,INT strper)						//力量资质	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSmartnessPer(INT nIndex,INT conper)						//敏捷资质	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetMindPer(INT nIndex,INT dexper)						//智力资质
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetConstitutionPer(INT nIndex,INT intper)						//体质资质
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL HumanItemExtraContainer::SetSavvy(INT nIndex,INT iSavvy)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetGenGu(INT nIndex,INT gengu)							//根骨
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetGrowRate(INT nIndex,FLOAT rate)						//成长率
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetRemainPoint(INT nIndex,INT rPoint)					//一级属性剩余点数
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetExp(INT nIndex,INT exp)							//经验值
{	
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//基础一级战斗属性（不包括技能和装备增加的部分）
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//宠物技能
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemExtraContainer::SetDBDirty(INT OffSet)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	Assert(OffSet>=0);
	Assert(OffSet<m_nContainerSize);

	switch(m_ICTType)
	{
	case ICT_EXTRA_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+m_DBOffSet+OffSet)); 
		}
		break;
	default:
		{
			AssertEx(FALSE,"无效容器类型");
			return FALSE;
		}
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
