
#include "StdAfx.h"
#include "GMActionSystem_LifeAbility.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDP_CharacterData.h"
#include "Event\GMEventSystem.h"
#include "GIException.h"
#include "Procedure\GameProcedure.h"


////////////////////////////////////////////////////////////////////////////////////
CActionItem_LifeAbility::CActionItem_LifeAbility(INT nID)
:CActionItem(nID)
,m_bStudy(false)
,m_iLevel(0)
,m_pSclient_lifeability(NULL)
{
}

CActionItem_LifeAbility::~CActionItem_LifeAbility()
{
	if( !m_pSclient_lifeability )
	{
		delete m_pSclient_lifeability;
		m_pSclient_lifeability = NULL;
	}
}


VOID CActionItem_LifeAbility::Update_LifeAbility(const SCLIENT_LIFEABILITY* pAbility)
{
	KLAssert(pAbility);

	//引用
	m_idLifeAbilityImpl = pAbility->m_pDefine->nID;
	//名称
	m_strName = pAbility->m_pDefine->szName;
	//图标
	m_strIconName = pAbility->m_pDefine->szIconName;

	m_iLevel = pAbility->m_nLevel;

	//通知UI
	UpdateToRefrence();
}

const SCLIENT_LIFEABILITY* CActionItem_LifeAbility::GetLifeAbilityImpl(VOID)
{
	if( m_bStudy )
		return m_pSclient_lifeability;
	else
		return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_idLifeAbilityImpl);
}

INT	CActionItem_LifeAbility::GetDefineID(VOID)
{
	const SCLIENT_LIFEABILITY* pAbility = GetLifeAbilityImpl();
	if(!pAbility) return -1;

	return pAbility->m_pDefine->nID;
}

LPCTSTR CActionItem_LifeAbility::GetDesc(VOID)
{
	return GetName();
}

INT CActionItem_LifeAbility::GetPosIndex(VOID)
{
	const SCLIENT_LIFEABILITY* pLifeAbility = GetLifeAbilityImpl();
	if(!pLifeAbility) return -1;

	return pLifeAbility->m_nPosIndex;
}

VOID CActionItem_LifeAbility::DoAction(VOID)
{
	const SCLIENT_LIFEABILITY* pAbility = (const SCLIENT_LIFEABILITY*)GetLifeAbilityImpl();
	if(!pAbility) return;

	//如果这个时候交易栏开着，那么需要强行关闭，不能同时进行这两个操作
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("Exchange"))
	{
		CEventSystem::GetMe()->PushEvent(GE_CANCEL_EXCHANGE);
	}

	CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_ITEM, pAbility->m_pDefine->nID);
	/*
	switch(pAbility->m_pDefine->nID)
	{
	case 1:		//烹饪模块
	case 2:		//制药模块
	case 4:		//铸剑模块
	//	case 3:		//加工模块
	case 5:		//制衣模块
	case 6:		//艺术制作模块
	case 7:		//工程学
	CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_ITEM, pAbility->m_pDefine->nID);
	break;
	//	case 3:		//加工模块
	//		CEventSystem::GetMe()->PushEvent(GE_OPEN_COMPOSE_GEM,1);
	break;
	}
	*/
}

VOID CActionItem_LifeAbility::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{

}
