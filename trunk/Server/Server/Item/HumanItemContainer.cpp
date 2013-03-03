#include "stdafx.h"
#include "HumanItemContainer.h"
#include "HumanDB.h"

BOOL HumanItemContainer::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

	if(!ItemContainer::Init(pInit))
		return FALSE;

	const _HUMAN_ITEMCONTAINER_INIT*	pHICInit = 
		static_cast<const _HUMAN_ITEMCONTAINER_INIT*>(pInit);

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

UINT	HumanItemContainer::ConIndex2BagIndex(UINT uConIndex)
{
	__ENTER_FUNCTION

	Assert(uConIndex!=INVALID_INDEX);

	return m_DBOffSet+uConIndex;

	__LEAVE_FUNCTION

	return INVALID_INDEX;
}

UINT	HumanItemContainer::BagIndex2ConIndex(UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex>=m_DBOffSet);
		Assert(uBagIndex<m_DBOffSet+m_nContainerSize);

		return	uBagIndex-m_DBOffSet;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}

BOOL	HumanItemContainer::IsInContainer(UINT uBagIndex)
{
	__ENTER_FUNCTION
		if(uBagIndex >= m_DBOffSet && uBagIndex< GetContainerSize()+m_DBOffSet )
			return TRUE;
		else
			return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemContainer::ExpandContainerSize(UINT uSzie)
{
    __ENTER_FUNCTION

        if(!ItemContainer::ExpandSize(uSzie))
            return FALSE;

        return TRUE;
    __LEAVE_FUNCTION
        return FALSE;
}

BOOL	HumanItemContainer::SetContainerSize(UINT uSzie)
{
    __ENTER_FUNCTION

        if(!ItemContainer::SetSize(uSzie))
            return FALSE;

    return TRUE;
    __LEAVE_FUNCTION
        return FALSE;
}

INT		HumanItemContainer::GetContainerType()
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

BOOL	HumanItemContainer::SetItem(const INT nIndex, const Item* pItem )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
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
BOOL		HumanItemContainer::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	
	BOOL	bRet = ItemContainer::SetItemLayCount(nIndex,nCount);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return	bRet;
	__LEAVE_FUNCTION

		return  FALSE;

}
//������Ʒ�ص�����
BOOL		HumanItemContainer::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION

	BOOL	bRet	=	ItemContainer::DecItemLayCount(nIndex,nCount);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
//������Ʒ�ص�����
BOOL		HumanItemContainer::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL	bRet  = ItemContainer::IncItemLayCount(nIndex,nCount);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

//ɾ����Ʒ
BOOL		HumanItemContainer::EraseItem(UINT uIndex)
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
BOOL			HumanItemContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
	
		Assert(m_pHumanDB);
		
		BOOL	bRet	=	ItemContainer::SetItemDur(nIndex,nDur);
	
		if(bRet)
		{
			SetDBDirty(nIndex);
		}

		return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
//������Ʒ���˵�
BOOL		HumanItemContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION
	
		Assert(m_pHumanDB);
		
		BOOL	bRet	=	ItemContainer::SetItemDamagePoint(nIndex,nPoint);
	
		if(bRet)
		{
			SetDBDirty(nIndex);
		}

		return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


//������Ʒ����;ö�
BOOL			HumanItemContainer::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL	bRet	=	ItemContainer::SetItemMaxDur(nIndex,nDur);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL		HumanItemContainer::SetItemCurMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	BOOL	bRet	=	ItemContainer::SetItemCurMaxDur(nIndex,nDur);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanItemContainer::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION
	
	Assert(m_pHumanDB);
	Assert(pItem);
	
	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pItem);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL		HumanItemContainer::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::SetItemBind(nIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL		HumanItemContainer::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	BOOL bRet = ItemContainer::SetItemIdent(nIndex);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanItemContainer::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

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
BOOL		HumanItemContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::AddItemAttr(nIndex, iA);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//ɾ����Ʒ����
BOOL		HumanItemContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::DelItemAttr(nIndex, iA);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//ɾ����ʯ��Ϣ
BOOL		HumanItemContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::DelGemInfo(nIndex, GemIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//��ӱ�ʯ��Ϣ
BOOL		HumanItemContainer::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
{
__ENTER_FUNCTION

	BOOL bRet = ItemContainer::AddGemInfo(nIndex, GemIndex, GemType);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//-----------------------------------------------------------------------------------------
/*
*	���� ʹ�÷���
*/
INT		HumanItemContainer::GetIndexByGUID( const PET_GUID_t* pGuid )
{
	__ENTER_FUNCTION
		Assert(0);
	return INVALID_INDEX;
	__LEAVE_FUNCTION
		return INVALID_INDEX;
}

BOOL	 HumanItemContainer::SetItemGuid(INT nIndex,PET_GUID_t* pGUID)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	 HumanItemContainer::SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetDataID(INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetName(INT nIndex,const CHAR* pName)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetNick(INT nIndex,const CHAR* pNick)						//�����ǳ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetLevel(INT nIndex,INT level)							//���õȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//����Я���ȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetAttackType(INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetAIType(INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetPetType(INT nIndex,BYTE PetType)					//���������죬Ұ��
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetGeneration(INT nIndex,BYTE Gen)						//������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetStrengthPer(INT nIndex,INT strper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetSmartnessPer(INT nIndex,INT conper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetMindPer(INT nIndex,INT dexper)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetConstitutionPer(INT nIndex,INT intper)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL HumanItemContainer::SetSavvy(INT nIndex,INT iSavvy)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetGenGu(INT nIndex,INT gengu)							//����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetGrowRate(INT nIndex,FLOAT rate)						//�ɳ���
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetRemainPoint(INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetExp(INT nIndex,INT exp)							//����ֵ
{	
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//���＼��
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemContainer::SetDBDirty(INT OffSet)
{
	__ENTER_FUNCTION


	Assert(m_pHumanDB);
	Assert(OffSet>=0);
	Assert(OffSet<m_nContainerSize);

	switch(m_ICTType)
	{
	case ICT_BASE_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+BASE_CONTAINER_OFFSET+OffSet)); 
		}
		break;
	/*case ICT_MAT_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+MAT_CONTAINER_OFFSET+OffSet)); 
		}
		break;
	case ICT_TASK_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+TASK_CONTAINER_OFFSET+OffSet)); 
		}
		break;*/
	case ICT_EQUIP_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+EQUIP_CONTAINER_OFFSET+OffSet)); 
		}
		break;
	case ICT_BANK_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+BANK_CONTAINER_OFFSET+OffSet)); 
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


