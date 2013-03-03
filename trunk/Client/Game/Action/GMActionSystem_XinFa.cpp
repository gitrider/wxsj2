
#include "StdAfx.h"
#include "GMActionSystem_XinFa.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Event\GMEventSystem.h"
#include "GIException.h"

// $$
//
//CActionItem_XinFa::CActionItem_XinFa(INT nID) : CActionItem(nID)
//{
//}
//
//CActionItem_XinFa::~CActionItem_XinFa()
//{
//}
//
//
//VOID CActionItem_XinFa::Update_XinFa(const SCLIENT_XINFA* pXinfaDef)
//{
//	KLAssert(pXinfaDef);
//
//	//引用
//	m_idXinfa = pXinfaDef->m_pDefine->nID;
//	//名称
//	m_strName = pXinfaDef->m_pDefine->pszName;
//	//图标
//	m_strIconName = pXinfaDef->m_pDefine->pszIconName;
//
//	//通知UI
//	UpdateToRefrence();
//}
//
////const SCLIENT_XINFA* CActionItem_XinFa::GetXinFaImpl(VOID)
////{
////	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(m_idXinfa);
////}
//
//INT	CActionItem_XinFa::GetDefineID(VOID)
//{
//	const SCLIENT_XINFA* pXinfa = GetXinFaImpl();
//	if(!pXinfa) return -1;
//	
//	return pXinfa->m_pDefine->nID;
//}
//
//LPCTSTR CActionItem_XinFa::GetDesc(VOID)
//{
//	const SCLIENT_XINFA* pXinfa = GetXinFaImpl();
//	if(!pXinfa) return "ERROR";
//
//	//static CHAR szTemp[MAX_PATH];
//	//_snprintf(szTemp, MAX_PATH, "%s\n------\n%s\n%d级\n%s", 
//	//	pXinfa->m_pDefine->pszName,
//	//	pXinfa->m_pDefine->pszDesc, 
//	//	pXinfa->m_nLevel,
//	//	pXinfa->m_bLeaned ? "学会" :"尚未学会");//
//	return pXinfa->m_pDefine->pszDesc;
//
//}
//
//INT CActionItem_XinFa::GetPosIndex(VOID)
//{
//	const SCLIENT_XINFA* pXinfa = GetXinFaImpl();
//	if(!pXinfa) return -1;
//
//	return pXinfa->m_nPosIndex;
//}
//
//VOID CActionItem_XinFa::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
//{
//}
//
//// 得到心法等级
//INT	CActionItem_XinFa::GetXinfaLevel()
//{
//	const SCLIENT_XINFA* pXinfa = GetXinFaImpl();
//	if(!pXinfa) 
//	{
//		return 0;
//	};
//
//	return pXinfa->m_nLevel;
//}
