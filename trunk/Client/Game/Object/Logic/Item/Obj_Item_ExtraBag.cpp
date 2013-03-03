
#include "StdAfx.h"
#include "Obj_Item_ExtraBag.h"
#include "GIException.h"



CObject_Item_ExtraBag::CObject_Item_ExtraBag(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_ExtraBag::~CObject_Item_ExtraBag()
{
}


VOID CObject_Item_ExtraBag::AsExtraBag(const _DBC_ITEM_EXTRABAG* pStoreMapDefine)
{
	KLAssert(pStoreMapDefine);

	m_theBaseDef = pStoreMapDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100  + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

//�õ���Ʒ�����
ITEM_CLASS CObject_Item_ExtraBag::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}

// �õ�type��Ϣ
INT	CObject_Item_ExtraBag::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}

//ͼ����
LPCTSTR CObject_Item_ExtraBag::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
}

// �õ���Ʒ����ļ۸�
INT CObject_Item_ExtraBag::GetItemBasePrice()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nBasePrice;
	}
	return 0;
}

// �õ�������
INT CObject_Item_ExtraBag::GetItemPrice()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nSalePrice;
	}
	return 0;
}

//�õ���Чʱ��
INT CObject_Item_ExtraBag::GetEffectTime()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nEffectTime;
	}
	return -1;
}

//�õ���Ч������
INT CObject_Item_ExtraBag::GetEffectGridNum()
{	
	if (m_theBaseDef)
	{
		m_theBaseDef->nEffectGridNum;
	}
	return -1;
}

// �õ���Ʒ���������� 
LPCTSTR CObject_Item_ExtraBag::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDesc;
	}
	return NULL;
}

//�õ�����ʱ��ģ��id
LPCTSTR CObject_Item_ExtraBag::GetDropVisualID(VOID)	const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nDropModelId;
	}
	return NULL;
}

//��ʾ������ɫ
LPCTSTR CObject_Item_ExtraBag::GetDropVisColor(VOID) const 
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDropVisColor;
	}
	return NULL;
}


//������ϸ����
VOID CObject_Item_ExtraBag::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_ExtraBag::GetExtraDesc(VOID)
{
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
}
 