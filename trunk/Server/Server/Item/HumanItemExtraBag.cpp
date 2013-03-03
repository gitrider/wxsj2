#include "stdafx.h"
#include "HumanItemExtraBag.h"
#include "HumanDB.h"

BOOL HumanItemExtraBag::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

		if(!ItemContainer::Init(pInit))
			return FALSE;

	const _HUMAN_ITEMEXTRABAG_INIT*	pHICInit = 
		static_cast<const _HUMAN_ITEMEXTRABAG_INIT*>(pInit);

	if(pHICInit==NULL)
		return FALSE;

	Assert(pHICInit->m_pHumanDB);
	Assert(pHICInit->m_pExtraBagItem);
	m_ICTType	=	pHICInit->m_ICTType;
	m_pHumanDB	=	pHICInit->m_pHumanDB;
	m_pExtraBagItem = pHICInit->m_pExtraBagItem;
	m_DBOffSet	=	pHICInit->m_DBOffSet;
	return TRUE;

	__LEAVE_FUNCTION

		return	FALSE;
}

UINT	HumanItemExtraBag::ConIndex2BagIndex(UINT uConIndex)
{
	__ENTER_FUNCTION

		Assert(uConIndex<(UINT)GetContainerSize());

	return m_DBOffSet+uConIndex;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}

UINT	HumanItemExtraBag::BagIndex2ConIndex(UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex>=m_DBOffSet);
	Assert(uBagIndex<m_DBOffSet+GetContainerSize());

	return	uBagIndex-m_DBOffSet;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}

BOOL	HumanItemExtraBag::IsInContainer(UINT uBagIndex)
{
	__ENTER_FUNCTION
		Assert(IsValid());
		Assert(m_pExtraBagItem->GetItemClass() == ICLASS_EXTRABAG);
		if(uBagIndex >= m_DBOffSet && uBagIndex< GetContainerSize()+m_DBOffSet )
			return TRUE;
		else
			return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL HumanItemExtraBag::IsCanUse()
{
	__ENTER_FUNCTION
		Assert(IsValid());
		Assert(m_pExtraBagItem->GetItemClass() == ICLASS_EXTRABAG);

		if (m_pExtraBagItem->GetExtraBagRemainTime() == 0)
		{
			return FALSE;
		}
		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
}

BOOL HumanItemExtraBag::IsValid()
{
	__ENTER_FUNCTION
		if (m_pExtraBagItem->IsEmpty())
		{
			return FALSE;
		}
		Assert(m_pExtraBagItem->GetItemClass() == ICLASS_EXTRABAG);

		return TRUE;
		__LEAVE_FUNCTION
			return FALSE;
}

INT HumanItemExtraBag::GetContainerType()
{
	__ENTER_FUNCTION

		return m_ICTType;

	__LEAVE_FUNCTION

		return m_ICTType;
}

UINT	HumanItemExtraBag::CalcItemSpace()
{
	__ENTER_FUNCTION
		Assert(IsValid());
		if (!IsCanUse())
		{
			return 0;
		}

		return ItemContainer::CalcItemSpace();

		__LEAVE_FUNCTION

			return 0;
}

//�ж������Ƿ��ǿ�
BOOL	HumanItemExtraBag::IsEmpty()
{
	__ENTER_FUNCTION
		Assert(IsValid());

		return ItemContainer::IsEmpty();

		__LEAVE_FUNCTION

			return FALSE;
}

INT	  HumanItemExtraBag::GetContainerSize()
{
	__ENTER_FUNCTION
		Assert(IsValid());
		Assert(m_pExtraBagItem->GetItemClass() == ICLASS_EXTRABAG);
		return m_pExtraBagItem->GetExtraBagSize();

		__LEAVE_FUNCTION

			return 0;	
}

UINT	HumanItemExtraBag::CalcExchangeItemSpace()
{
	__ENTER_FUNCTION
		Assert(IsValid());
		if (!IsCanUse())
		{
			return 0;
		}

		return ItemContainer::CalcExchangeItemSpace();

		__LEAVE_FUNCTION

			return 0;
}

INT		HumanItemExtraBag::GetEmptyItemIndex( )
{
	__ENTER_FUNCTION
		Assert(IsValid());

		if (!IsCanUse())
		{
			return INVALID_INDEX;
		}

		return ItemContainer::GetEmptyItemIndex();

		__LEAVE_FUNCTION

			return INVALID_INDEX ;
}

BOOL HumanItemExtraBag::SetItemPWLock(INT nIndex,BOOL bLock)
{
	__ENTER_FUNCTION
		if (!IsInContainer(ConIndex2BagIndex(nIndex)))
		{
			Assert(0);
			return FALSE;
		}

		if (!IsCanUse())
		{
			return FALSE;
		}

		return ItemContainer::SetItemPWLock(nIndex, bLock);

		__LEAVE_FUNCTION

			return FALSE;
}

INT HumanItemExtraBag::GetIndexByType( const UINT uType )
{
	__ENTER_FUNCTION
		Assert(IsValid());
		if (!IsCanUse())
		{
			return INVALID_INDEX;
		}

		return ItemContainer::GetIndexByType(uType);
		__LEAVE_FUNCTION

			return INVALID_INDEX ;
}

INT HumanItemExtraBag::GetIndexByType( const UINT uType ,UINT emptyCount)
{
	__ENTER_FUNCTION
		Assert(IsValid());
		if (!IsCanUse())
		{
			return INVALID_INDEX;
		}

		return ItemContainer::GetIndexByType(uType, emptyCount);

		__LEAVE_FUNCTION

			return INVALID_INDEX ;
}

Item* HumanItemExtraBag::GetItem( const INT nIndex )
{
	__ENTER_FUNCTION
		if (!IsInContainer(ConIndex2BagIndex(nIndex)))
		{
			Assert(0);
			return NULL;
		}

		return ItemContainer::GetItem(nIndex);

		__LEAVE_FUNCTION

			return NULL ;
}

INT		HumanItemExtraBag::GetNoFullItemIndex(UINT uItemIndex)
{
	__ENTER_FUNCTION
		Assert(IsValid());

		if (!IsCanUse())
		{
			return INVALID_INDEX;
		}

		return ItemContainer::GetNoFullItemIndex(uItemIndex);

		__LEAVE_FUNCTION

			return -1;
}

BOOL	HumanItemExtraBag::ChangeSize(const INT nSize)
{
	__ENTER_FUNCTION
		Assert(nSize>=0);

		if (nSize==0)
		{
			Assert(m_pExtraBagItem->IsEmpty());
			return FALSE;
		}

		Assert(m_pExtraBagItem->GetExtraBagSize() == nSize);

		if (nSize>MAX_ITEMCONTAINER_SIZE)
		{
			Assert(0);
		}

		if (!IsEmpty())
		{
			Assert(0);
		}
		
		m_nContainerSize = nSize;
		__LEAVE_FUNCTION

			return FALSE;	
}
//-----------------------------------------------------------------------------------------
/*
*	��Ʒ ʹ�÷���
*/

BOOL	HumanItemExtraBag::SetItem(const INT nIndex, const Item* pItem )
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	Assert(pItem);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}
	
	if (!IsCanUse())
	{
		return FALSE;
	}

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
BOOL		HumanItemExtraBag::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

	ItemContainer::SetItemLayCount(nIndex,nCount);

	SetDBDirty(nIndex);
	return TRUE;

	__LEAVE_FUNCTION
return FALSE;
}
//������Ʒ�ص�����
BOOL		HumanItemExtraBag::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}
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
BOOL		HumanItemExtraBag::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

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
BOOL		HumanItemExtraBag::EraseItem(UINT uIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(uIndex)))
	{
		Assert(0);
		return FALSE;
	}
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
BOOL			HumanItemExtraBag::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
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
BOOL		HumanItemExtraBag::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
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
BOOL			HumanItemExtraBag::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

	BOOL	bRet	=	ItemContainer::SetItemMaxDur(nIndex,nDur);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemExtraBag::SetItemCurMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(m_pHumanDB);

	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

	BOOL	bRet	=	ItemContainer::SetItemCurMaxDur(nIndex,nDur);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanItemExtraBag::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	Assert(pItem);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pItem);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL		HumanItemExtraBag::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
	BOOL bRet = ItemContainer::SetItemBind(nIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanItemExtraBag::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
	BOOL bRet = ItemContainer::SetItemIdent(nIndex);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL HumanItemExtraBag::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
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
BOOL		HumanItemExtraBag::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

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
BOOL		HumanItemExtraBag::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}
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
BOOL		HumanItemExtraBag::DelGemInfo(INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}

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
BOOL		HumanItemExtraBag::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
{
	__ENTER_FUNCTION

	if (!IsInContainer(ConIndex2BagIndex(nIndex)))
	{
		Assert(0);
		return FALSE;
	}

	if (!IsCanUse())
	{
		return FALSE;
	}


	BOOL bRet = ItemContainer::AddGemInfo(nIndex, GemIndex, GemType);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

INT HumanItemExtraBag::GetIndexByGUID( const _ITEM_GUID* pGuid )
{
	__ENTER_FUNCTION

		Assert(IsValid());

	return ItemContainer::GetIndexByGUID(pGuid) ;
		__LEAVE_FUNCTION

			return INVALID_INDEX ;
}
//-----------------------------------------------------------------------------------------
/*
*	���� ʹ�÷���
*/
INT HumanItemExtraBag::GetIndexByGUID( const PET_GUID_t* pGuid )
{
	__ENTER_FUNCTION
		Assert(0);
	return INVALID_INDEX;
	__LEAVE_FUNCTION
		return INVALID_INDEX;
}

BOOL HumanItemExtraBag::SetItemGuid(INT nIndex,PET_GUID_t* pGUID)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	 HumanItemExtraBag::SetItemValue(INT nIndex,const _PET_DB_LOAD*	pPet)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetPetGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetSpouseGUID(INT nIndex,PET_GUID_t GUID) 				//���ó���GUID
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetDataID(INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetName(INT nIndex,const CHAR* pName)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetNick(INT nIndex,const CHAR* pNick)						//�����ǳ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetLevel(INT nIndex,INT level)							//���õȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetTakeLevel(INT nIndex,INT takeLevel)					//����Я���ȼ�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetAttackType(INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetAIType(INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetPetType(INT nIndex,BYTE PetType)					//���������죬Ұ��
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetGeneration(INT nIndex,BYTE Gen)						//������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetHappiness(INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetStrengthPer(INT nIndex,INT strper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetSmartnessPer(INT nIndex,INT conper)						//��������	
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetMindPer(INT nIndex,INT dexper)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetConstitutionPer(INT nIndex,INT intper)						//��������
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL HumanItemExtraBag::SetSavvy(INT nIndex,INT iSavvy)
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetGenGu(INT nIndex,INT gengu)							//����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetGrowRate(INT nIndex,FLOAT rate)						//�ɳ���
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetRemainPoint(INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetExp(INT nIndex,INT exp)							//����ֵ
{	
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL  HumanItemExtraBag::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//���＼��
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemExtraBag::SetDBDirty(INT OffSet)
{
	__ENTER_FUNCTION


		Assert(m_pHumanDB);
	Assert(OffSet>=0);
	Assert(OffSet<m_nContainerSize);

	switch(m_ICTType)
	{
	case ICT_BASEBAG_CONTAINER:
	case ICT_EXTRABAG_CONTAINER:
	case ICT_BASE_CONTAINER:
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