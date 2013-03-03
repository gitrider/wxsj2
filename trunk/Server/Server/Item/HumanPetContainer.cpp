#include "stdafx.h"
#include "HumanPetContainer.h"
#include "HumanDB.h"

BOOL HumanPetContainer::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

	if(!ItemContainer::Init(pInit))
		return FALSE;

	const _HUMAN_PETCONTAINER_INIT*	pHICInit = 
		static_cast<const _HUMAN_PETCONTAINER_INIT*>(pInit);

	if(pHICInit==NULL)
		return FALSE;

	Assert(pHICInit->m_pHumanDB);

	m_ICTType	=	pHICInit->m_ICTType;
	m_pHumanDB	=	pHICInit->m_pHumanDB;
	
	return TRUE;
	
	__LEAVE_FUNCTION

	return	FALSE;
}


INT		HumanPetContainer::GetContainerType()
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
BOOL	HumanPetContainer::SetItem(const INT nIndex, const Item* pItem )
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
BOOL	HumanPetContainer::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION
		Assert(0);
	return FALSE;
	__LEAVE_FUNCTION
		return FALSE;
}

//������Ʒ�ص�����
BOOL	HumanPetContainer::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}
//������Ʒ�ص�����
BOOL	HumanPetContainer::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����Ʒ
BOOL	HumanPetContainer::EraseItem(UINT uIndex)
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
BOOL	HumanPetContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//������Ʒ���˵�
BOOL		HumanPetContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}


//������Ʒ����;ö�
BOOL	HumanPetContainer::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL	HumanPetContainer::SetItemCurMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL		HumanPetContainer::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}


BOOL		HumanPetContainer::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);
	//Assert(nIndex<m_DBOffSet+m_nContainerSize);

	BOOL bRet = ItemContainer::SetItemBind(nIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanPetContainer::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

BOOL	HumanPetContainer::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}


//������Ʒ����
BOOL		HumanPetContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����Ʒ����
BOOL		HumanPetContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//ɾ����ʯ��Ϣ
BOOL		HumanPetContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION
		Assert(0);
	return false;
	__LEAVE_FUNCTION
		return false;
}

//��ӱ�ʯ��Ϣ
BOOL		HumanPetContainer::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
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
BOOL		HumanPetContainer::SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION
	
	Assert(m_pHumanDB);
	Assert(pPet);
	
	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pPet);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID)				//���ó���GUID
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	BOOL bRet	=	ItemContainer::SetPetGUID(nIndex,GUID);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	BOOL bRet	=	ItemContainer::SetSpouseGUID(nIndex,GUID);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	HumanPetContainer::SetDataID(INT nIndex,INT ID)							//���ó���ģ��		
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetDataID(nIndex,ID);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL 	HumanPetContainer::SetName(INT nIndex, const CHAR* pName)						//��������
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetName(nIndex,pName);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL   	HumanPetContainer::SetNick(INT nIndex, const CHAR* pNick)						//�����ǳ�
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetNick(nIndex,pNick);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetLevel(INT nIndex,INT level)							//���õȼ�
{

	__ENTER_FUNCTION
	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetLevel(nIndex,level);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//����Я���ȼ�
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetTakeLevel(nIndex,takeLevel);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetAttackType(INT nIndex,INT attackType)				//���ý������ͣ���/����
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetAttackType(nIndex,attackType);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetAIType(INT nIndex,INT AIType)						//����AI����
{
	__ENTER_FUNCTION

		Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetAIType(nIndex,AIType);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//������Ӫ
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetCampData(nIndex,pCamp);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

//BOOL  	HumanPetContainer::SetHP(INT nIndex,INT hp)								//��������ֵ
//{
//	__ENTER_FUNCTION
//
//	//Assert(m_pHumanDB);
//
//	//BOOL bRet	=	ItemContainer::SetHP(nIndex,hp);
//
//	//if(bRet)
//	//{
//	//	SetDBDirty(nIndex);
//	//}
//
//	//return bRet;
//	return TRUE;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//}

//BOOL  	HumanPetContainer::SetLife(INT nIndex,INT Life)							//���õ�ǰ����
//{
//	__ENTER_FUNCTION
//
//	Assert(m_pHumanDB);
//
//	BOOL bRet	=	ItemContainer::SetLife(nIndex,Life);
//
//	if(bRet)
//	{
//		SetDBDirty(nIndex);
//	}
//
//	return bRet;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//}
//BOOL  	HumanPetContainer::SetPetType(INT nIndex,BYTE PetType)				//���������죬Ұ��
//{
//	__ENTER_FUNCTION
//
//	Assert(m_pHumanDB);
//
//	BOOL bRet	=	ItemContainer::SetPetType(nIndex,PetType);
//
//	if(bRet)
//	{
//		SetDBDirty(nIndex);
//	}
//
//	return bRet;
//
//	__LEAVE_FUNCTION
//
//	return FALSE;
//}

BOOL  	HumanPetContainer::SetGeneration(INT nIndex,BYTE Gen)						//������
{	
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGeneration(nIndex,Gen);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;

}
BOOL  	HumanPetContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//���ֶ�		
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetHappiness(nIndex,byHappiness);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
//��������
BOOL  	HumanPetContainer::SetStrengthPer(INT nIndex,INT strper)						
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetStrengthPer(nIndex,strper);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
//��������
BOOL  	HumanPetContainer::SetSmartnessPer(INT nIndex,INT conper)
{	
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetSmartnessPer(nIndex,conper);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

//��������
BOOL   	HumanPetContainer::SetMindPer(INT nIndex,INT dexper)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetMindPer(nIndex,dexper);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

//��������
BOOL   	HumanPetContainer::SetConstitutionPer(INT nIndex,INT intper)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetConstitutionPer(nIndex,intper);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanPetContainer::SetGenGu(INT nIndex,INT gengu)							//����
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGenGu(nIndex,gengu);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetGrowRate(INT nIndex,FLOAT rate)						//�ɳ���
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetGrowRate(nIndex,rate);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  	HumanPetContainer::SetRemainPoint(INT nIndex,INT rPoint)					//һ������ʣ�����
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetRemainPoint(nIndex,rPoint);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanPetContainer::SetMatingFlag( INT iIndex, BOOL bFlag )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetMatingFlag(iIndex,bFlag);

	if(bRet)
	{
		SetDBDirty(iIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanPetContainer::SetMatingStartTime( INT nIndex, UINT uTime )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetMatingStartTime(nIndex,uTime);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanPetContainer::SetMatingFinishFlag( INT nIndex, BOOL bFlag )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetMatingFinishFlag(nIndex,bFlag);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL HumanPetContainer::SetMatingLevel( INT nIndex, INT iLevel )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetMatingLevel(nIndex,iLevel);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanPetContainer::SetPetSex( INT nIndex, INT iSex )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetPetSex(nIndex,iSex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanPetContainer::SetCheckUpFlag( INT nIndex, BOOL bCheckup )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetCheckUpFlag(nIndex,bCheckup);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetExp(INT nIndex,INT exp)							//����ֵ
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetExp(nIndex,exp);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	HumanPetContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//����һ��ս�����ԣ����������ܺ�װ�����ӵĲ��֣�
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetLvl1Attr(nIndex,type,value);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	HumanPetContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//���＼��
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL bRet	=	ItemContainer::SetSkill(nIndex,SkillIndex,skill);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanPetContainer::SetDBDirty(INT index)
{
	Assert(m_pHumanDB);
	switch(m_ICTType)
	{
	case ICT_HUMAN_PET_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)CATTR_DB_PET); 
		}
		break;

	default:
		{
			AssertEx(FALSE,"��Ч��������");
			return FALSE;
		}
	}
	return TRUE;
}

