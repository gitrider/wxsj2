
#include "StdAfx.h"
#include "Obj_Item_StoreMap.h"
#include "GIException.h"



CObject_Item_StoreMap::CObject_Item_StoreMap(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_StoreMap::~CObject_Item_StoreMap()
{
}


VOID CObject_Item_StoreMap::AsStoreMap(const _DBC_ITEM_STOREMAP* pStoreMapDefine)
{
	KLAssert(pStoreMapDefine);

	m_theBaseDef = pStoreMapDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100  + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

LPCTSTR  CObject_Item_StoreMap::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//������ϸ����
VOID CObject_Item_StoreMap::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const STORE_MAP_INFO& infoStoreMap = pItemInfo->m_StoreMap;

	SetNumber(pItemInfo->GetItemCount());
	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_StoreMap::GetExtraDesc(VOID)
{
	//_snprintf(szTemp, 1024, 
	//	"ID=%d\n"
	//	"����: %s(%s)\n"
	//	"����ȼ�: %d\n"
	//	"����: %d\n"
	//	"-----------\n"
	//	"%s\n",
	//	m_ID.m_idOrg.m_uSerial,
	//	GetName(),	GetDesc(),
	//	m_theBaseDef->nLevelRequire,
	//	GetNumber(),
	//	m_theBaseDef->szDesc);//

	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
	
}

VOID  CObject_Item_StoreMap::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_StoreMap*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

//  ;
// �õ�quality��Ϣ //remove
//INT	CObject_Item_StoreMap::GetItemTableQuality(VOID)
//{
//	return m_theBaseDef->nQuality;
//}

//  ;
// �õ�type��Ϣ
INT	CObject_Item_StoreMap::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}

//��������

LPCTSTR	CObject_Item_StoreMap::GetDropVisualID(VOID)	const
 {
	return NULL;
 }
 //������ɫ

 LPCTSTR	CObject_Item_StoreMap::GetDropVisColor(VOID)	const
 {
	 return NULL;
 }

 INT		CObject_Item_StoreMap::GetNeedLevel()	
 {
	 if(m_theBaseDef)
	 {
		 return m_theBaseDef->nLevelRequire;
	 }
	 return -1;

 }

 