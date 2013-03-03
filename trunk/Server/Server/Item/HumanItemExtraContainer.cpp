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
*	��Ʒ ʹ�÷���
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

//������Ʒ�ص�����
BOOL	HumanItemExtraContainer::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION
	Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}
//������Ʒ�ص�����
BOOL	HumanItemExtraContainer::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}
//������Ʒ�ص�����
BOOL	HumanItemExtraContainer::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����Ʒ
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

//������Ʒ�;ö�
BOOL	HumanItemExtraContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//������Ʒ���˵�
BOOL		HumanItemExtraContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}


//������Ʒ����;ö�
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


//������Ʒ����
BOOL		HumanItemExtraContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����Ʒ����
BOOL		HumanItemExtraContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����ʯ��Ϣ
BOOL		HumanItemExtraContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//��ӱ�ʯ��Ϣ
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
*	���� ʹ�÷���
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

BOOL  HumanItemExtraContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetDataID(INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetName(INT nIndex,const CHAR* pName)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetNick(INT nIndex,const CHAR* pNick)						//�����ǳ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetLevel(INT nIndex,INT level)							//���õȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//����Я���ȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetAttackType(INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetAIType(INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetPetType(INT nIndex,BYTE PetType)					//���������죬Ұ��
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetGeneration(INT nIndex,BYTE Gen)						//������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetStrengthPer(INT nIndex,INT strper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSmartnessPer(INT nIndex,INT conper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetMindPer(INT nIndex,INT dexper)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetConstitutionPer(INT nIndex,INT intper)						//��������
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

BOOL  HumanItemExtraContainer::SetGenGu(INT nIndex,INT gengu)							//����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetGrowRate(INT nIndex,FLOAT rate)						//�ɳ���
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetRemainPoint(INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetExp(INT nIndex,INT exp)							//����ֵ
{	
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//���＼��
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
			AssertEx(FALSE,"��Ч��������");
			return FALSE;
		}
	}

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
