
#include "StdAfx.h"
#include "Obj_Item_SoulBead.h"
#include "GIException.h"



CObject_Item_SoulBead::CObject_Item_SoulBead(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;

	m_soulbeadTypeName.clear();
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_STR, "力道" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_DEX, "身法" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_INT, "内力" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_POINT_CON, "体质" ) );

	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_NEAR, "远程内攻" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_FAR, "远程外攻" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR, "近程内攻" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_ATTACK_MAGIC_FAR, "近程外攻" ) );

	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_MAGIC_NEAR, "近程内防" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_MAGIC_FAR, "近程外防" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_NEAR, "远程内防" ) );
	m_soulbeadTypeName.insert( std::make_pair( IATTRIBUTE_BASE_DEFENCE_FAR, "远程外防" ) );
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

//得到物品的类别
ITEM_CLASS CObject_Item_SoulBead::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}

// 得到type信息
INT	CObject_Item_SoulBead::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}

//图标名
LPCTSTR CObject_Item_SoulBead::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
}


//得到有效时间
INT CObject_Item_SoulBead::GetEffectTime()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nEffectTime;
	}
	return -1;
}

//得到是否广播
INT CObject_Item_SoulBead::GetIsRadio()
{	
	if (m_theBaseDef)
	{
		m_theBaseDef->nIsRadio;
	}
	return -1;
}

// 得到物品的类型描述 
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

//得到掉落时的模型id
LPCTSTR CObject_Item_SoulBead::GetDropVisualID(VOID)	const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nDropModelId;
	}
	return NULL;
}

//显示名称颜色
LPCTSTR CObject_Item_SoulBead::GetDropVisColor(VOID) const 
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDropVisColor;
	}
	return NULL;
}


//设置详细解释
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
	//		return "远程内攻";
	//	
	//	case IATTRIBUTE_BASE_ATTACK_FAR:
	//		return "远程外攻";

	//	case IATTRIBUTE_BASE_ATTACK_MAGIC_NEAR:
	//		return "近程内攻";

	//	case IATTRIBUTE_BASE_ATTACK_MAGIC_FAR:
	//		return "近程外攻";
	//	}
	//}
	//switch( type )
	//{
	//case 0:
	//	return GetItemTableType() == 0 ? "力道" : "近程外攻";

	//case 1:
	//	return GetItemTableType() == 0 ? "体质" : "远程外攻";

	//case 2:
	//	return GetItemTableType() == 0 ? "内力" : "近程内攻";

	//case 3:
	//	return GetItemTableType() == 0 ? "身法" : "远程内攻";
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
 