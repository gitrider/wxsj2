
#include "StdAfx.h"
#include "Obj_Item_SoulBead.h"
#include "GIException.h"



CObject_Item_SoulBead::CObject_Item_SoulBead(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;

	m_soulbeadTypeName.clear();
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_STR, "����" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_DEX, "��" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_INT, "����" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_CON, "����" ) );

	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_NEAR, "Զ���ڹ�" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_FAR, "Զ���⹥" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR, "�����ڹ�" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_MAGIC_FAR, "�����⹥" ) );

	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR, "�����ڷ�" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR, "�������" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_NEAR, "Զ���ڷ�" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_FAR, "Զ�����" ) );
}

CObject_Item_SoulBead::~CObject_Item_SoulBead()
{
}


VOID CObject_Item_SoulBead::AsSoulBead(const _DBC_ITEM_SOULBEAD* pStoreMapDefine)
{
	KLAssert(pStoreMapDefine);

	m_theBaseDef = pStoreMapDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100  + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

//�õ���Ʒ�����
ITEM_CLASS CObject_Item_SoulBead::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}

// �õ�type��Ϣ
INT	CObject_Item_SoulBead::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}

//ͼ����
LPCTSTR CObject_Item_SoulBead::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
}


//�õ���Чʱ��
INT CObject_Item_SoulBead::GetEffectTime()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nEffectTime;
	}
	return -1;
}

//�õ��Ƿ�㲥
INT CObject_Item_SoulBead::GetIsRadio()
{	
	if (m_theBaseDef)
	{
		m_theBaseDef->nIsRadio;
	}
	return -1;
}

// �õ���Ʒ���������� 
LPCTSTR CObject_Item_SoulBead::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szTypeDesc;
	}
	return NULL;
}

INT CObject_Item_SoulBead::GetSelectObjType()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nSelectObjType;
	}
	return -1;
}

//�õ�����ʱ��ģ��id
LPCTSTR CObject_Item_SoulBead::GetDropVisualID(VOID)	const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nDropModelId;
	}
	return NULL;
}

//��ʾ������ɫ
LPCTSTR CObject_Item_SoulBead::GetDropVisColor(VOID) const 
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDropVisColor;
	}
	return NULL;
}


//������ϸ����
VOID CObject_Item_SoulBead::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());

	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}

	memcpy( &m_soulbead, pItemInfo->GetSoulBeadData(), sizeof(SOUL_BEAD_INFO) );
}

LPCTSTR  CObject_Item_SoulBead::GetExtraDesc(VOID)
{
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
}

//INT CObject_Item_SoulBead::GetFecthHeadInfo_Type( INT Index )
//{
//	if( GetItemTableType() == 0 )
//	{
//		return m_soulbead.m_pWuSoulBeadAttr[Index].m_AttrType;
//	}
//	else if ( GetItemTableType() == 1 )
//	{
//		return m_soulbead.m_pQiSoulBeadAttr[Index].m_AttrType;
//	}
//
//	return -1;
//}

INT CObject_Item_SoulBead::GetFecthHeadInfo_Value( INT Index )
{
	if( GetItemTableType() == 0 )
	{
		return m_soulbead.m_pWuSoulBeadAttr[Index].m_Value.m_Value;
	}
	else if ( GetItemTableType() == 1 )
	{
		return m_soulbead.m_pQiSoulBeadAttr[Index].m_Value.m_Value;
	}


	return 0;
}

LPCTSTR CObject_Item_SoulBead::GetFecthHeadInfo_TypeDesc( INT Index )
{
	//if( GetItemTableType() == 0 )
	//{
	//	switch ( type )
	//	{
	//		
	//	}
	//}
	//else if ( GetItemTableType() == 1 )
	//{
	//	switch ( type )
	//	{
	//	case IATTRIBUTE_BASE_ATTACK_NEAR:
	//		return "Զ���ڹ�";
	//	
	//	case IATTRIBUTE_BASE_ATTACK_FAR:
	//		return "Զ���⹥";

	//	case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
	//		return "�����ڹ�";

	//	case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
	//		return "�����⹥";
	//	}
	//}
	//switch( type )
	//{
	//case 0:
	//	return GetItemTableType() == 0 ? "����" : "�����⹥";

	//case 1:
	//	return GetItemTableType() == 0 ? "����" : "Զ���⹥";

	//case 2:
	//	return GetItemTableType() == 0 ? "����" : "�����ڹ�";

	//case 3:
	//	return GetItemTableType() == 0 ? "��" : "Զ���ڹ�";
	//	
	//}
	
	INT type;
	if( GetItemTableType() == 0 )
	{
		type = m_soulbead.m_pWuSoulBeadAttr[Index].m_AttrType;
	}
	else if ( GetItemTableType() == 1 )
	{
		type = m_soulbead.m_pQiSoulBeadAttr[Index].m_AttrType;
	}

	return m_soulbeadTypeName[(ITEM_ATTRIBUTE)type].c_str();
}
 