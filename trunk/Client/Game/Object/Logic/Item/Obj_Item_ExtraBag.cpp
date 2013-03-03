
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

//得到物品的类别
ITEM_CLASS CObject_Item_ExtraBag::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}

// 得到type信息
INT	CObject_Item_ExtraBag::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
	return -1;
}

//图标名
LPCTSTR CObject_Item_ExtraBag::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
}

// 得到物品买入的价格
INT CObject_Item_ExtraBag::GetItemBasePrice()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nBasePrice;
	}
	return 0;
}

// 得到卖出价
INT CObject_Item_ExtraBag::GetItemPrice()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nSalePrice;
	}
	return 0;
}

//得到有效时间
INT CObject_Item_ExtraBag::GetEffectTime()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nEffectTime;
	}
	return -1;
}

//得到有效格子数
INT CObject_Item_ExtraBag::GetEffectGridNum()
{	
	if (m_theBaseDef)
	{
		m_theBaseDef->nEffectGridNum;
	}
	return -1;
}

// 得到物品的类型描述 
LPCTSTR CObject_Item_ExtraBag::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDesc;
	}
	return NULL;
}

//得到掉落时的模型id
LPCTSTR CObject_Item_ExtraBag::GetDropVisualID(VOID)	const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nDropModelId;
	}
	return NULL;
}

//显示名称颜色
LPCTSTR CObject_Item_ExtraBag::GetDropVisColor(VOID) const 
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szDropVisColor;
	}
	return NULL;
}


//设置详细解释
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
 