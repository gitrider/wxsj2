
//[7/8/2010 �¾���]
#include "StdAfx.h"
#include "Obj_Item_Mount.h"
#include "GIException.h"



CObject_Item_Mount::CObject_Item_Mount(INT id)
: CObject_Item(id), m_bIsBinded(FALSE), m_nObtainDate(0), m_bIsDue(FALSE), m_characterData(NULL), m_theBaseDef(NULL), m_nRemainTime(0)
{
}

CObject_Item_Mount::~CObject_Item_Mount()
{
}


VOID CObject_Item_Mount::AsMount(const _DBC_ITEM_MOUNT_ATT* pStoreMapDefine)
{
	KLAssert(pStoreMapDefine);
	m_theBaseDef = const_cast<_DBC_ITEM_MOUNT_ATT*>(pStoreMapDefine);

	DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
	m_characterData = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(m_theBaseDef->nCharMountID));

	//m_theBaseDef->szIcon = "xinshouzhuangnan";
	//m_theBaseDef->nClass = ICLASS_MOUNT; 
	//m_nParticularID = ( ( ICLASS_MOUNT * 100  + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

//�õ���Ʒ�����
ITEM_CLASS CObject_Item_Mount::GetItemClass(VOID) const 
{
	//if (m_theBaseDef)
	//	{
	//	return (ITEM_CLASS)m_theBaseDef->nClass; 
	//	}
	return ICLASS_MOUNT;
}

// �õ�type��Ϣ
INT	CObject_Item_Mount::GetItemTableType(VOID)
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->nType;
	//	}
	return -1;
}

//ͼ����
LPCTSTR CObject_Item_Mount::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
}

// �õ���Ʒ����ļ۸�
INT CObject_Item_Mount::GetItemBasePrice()
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->nBasePrice;
	//	}
	return 0;
}

// �õ�������
INT CObject_Item_Mount::GetItemPrice()
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->nSalePrice;
	//	}
	return 0;
}

//�õ���Чʱ��
INT CObject_Item_Mount::GetEffectTime()
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->nEffectTime;
	//	}
	return -1;
}

//�õ���Ч������
INT CObject_Item_Mount::GetEffectGridNum()
{	
	//if (m_theBaseDef)
	//	{
	//	m_theBaseDef->nEffectGridNum;
	//	}
	return -1;
}

// �õ���Ʒ���������� 
LPCTSTR CObject_Item_Mount::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDesc;
	}
	return NULL;
}

//�õ�����ʱ��ģ��id
LPCTSTR CObject_Item_Mount::GetDropVisualID(VOID)	const
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->nDropModelId;
	//	}
	return NULL;
}

//��ʾ������ɫ
LPCTSTR CObject_Item_Mount::GetDropVisColor(VOID) const 
{
	//if (m_theBaseDef)
	//	{
	//	return m_theBaseDef->szDropVisColor;
	//	}
	return NULL;
}


//������ϸ����
VOID CObject_Item_Mount::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_Mount::GetExtraDesc(VOID)
{
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
}
