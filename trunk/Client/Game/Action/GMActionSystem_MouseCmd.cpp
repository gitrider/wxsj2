#include "StdAfx.h"
#include "GMActionSystem_MouseCmd.h"
#include "NetWork/NetManager.h"
#include "..\DataPool\GMDataPool.h"
#include "GIObjectManager.h"
#include "..\Event\GMEventSystem.h"
#include "CGEquipSuitExchange.h"

#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool/GMUIDataPool.h"


CActionItem_MouseCmd_Repair* CActionItem_MouseCmd_Repair::s_pMe = 0;
CActionItem_MouseCmd_Repair::CActionItem_MouseCmd_Repair(INT id) : 
	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Repair";
}

CActionItem_MouseCmd_Repair::~CActionItem_MouseCmd_Repair()
{
	s_pMe = 0;
}


CActionItem_MouseCmd_Identify* CActionItem_MouseCmd_Identify::s_pMe = 0;
CActionItem_MouseCmd_Identify::CActionItem_MouseCmd_Identify(INT id) : 
	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Identify";
}

CActionItem_MouseCmd_Identify::~CActionItem_MouseCmd_Identify()
{
	s_pMe = 0;
}

VOID CActionItem_MouseCmd_Identify::SetDefaultState(BOOL bDefault)
{
	CActionItem::SetDefaultState(bDefault);

	if(bDefault)
	{
		//锁定物品
		tObject_Item* pInfo = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->m_nIdentifyPosition);
		if(pInfo)
		{
			pInfo->SetLock(TRUE);
		}

		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}
	else
	{
		// 取消锁定状态
		tObject_Item* pInfo = CDataPool::GetMe()->UserBag_GetItem(CDataPool::GetMe()->m_nIdentifyPosition);
		if(pInfo)
		{
			pInfo->SetLock(FALSE);
		}

		CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
	}
}



////////////////////////////////////////////////////////////////////////////////////
// CActionItem_ChangeSuit
////////////////////////////////////////////////////////////////////////////////////

CActionItem_ChangeSuit::CActionItem_ChangeSuit(INT nID)
:CActionItem(nID)
{
	m_idSuit = -1;
}

CActionItem_ChangeSuit::~CActionItem_ChangeSuit()
{
}

INT	CActionItem_ChangeSuit::GetDefineID(VOID)
{
	return m_idSuit;
}

LPCTSTR CActionItem_ChangeSuit::GetDesc(VOID)
{
	return GetName();
}

INT CActionItem_ChangeSuit::GetPosIndex(VOID)
{
	return 0;
}

VOID CActionItem_ChangeSuit::DoAction(VOID)
{
}

VOID CActionItem_ChangeSuit::SpecialOperation(void* pData)
{
	m_idSuit = *(INT*)pData;
	if( m_idSuit == -1 )
		return;

	Packets::CGEquipSuitExchange msg;
	msg.setEquipSuitNum(m_idSuit);
	CNetManager::GetMe()->SendPacket( &msg );
}

VOID CActionItem_ChangeSuit::UpdateSuit(STRING& iconName, INT suitId)
{
	m_strIconName = iconName;
	m_idSuit = suitId;
}

VOID CActionItem_ChangeSuit::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	if( bDestory )
	{
		CHAR cSourceType = szSourceName[0];
		switch(cSourceType)
		{
		case 'M':		//主菜单
			{
				INT nIndex = szSourceName[1]-'0';
				nIndex = szSourceName[2]-'0' + nIndex*10 ;

				CActionSystem::GetMe()->MainMenuBar_Set(nIndex, -1 );
			}
			break;
		default:
			break;
		}
	}

	//拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0') return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//如果不是拖到快捷栏，返回
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;


	INT nOldTargetId = -1;
	INT nIndex =-1;

	switch(cTargetType)
	{
	case 'M':		//主菜单
		{
			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;
			//查询目标位置原来的内容
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
			m_idSuit = ((CObject_Character*)CObjectManager::GetMe()->GetMySelf())->GetCharacterData()->Get_EquipSuitId();
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID() );
		}
		break;

	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M':
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10 ;
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
		}
		break;
	default:
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////////
// CActionItem_MouseCmd_Friend
////////////////////////////////////////////////////////////////////////////////////

CActionItem_MouseCmd_Friend* CActionItem_MouseCmd_Friend::s_pMe = 0;
CActionItem_MouseCmd_Friend::CActionItem_MouseCmd_Friend(INT id)
	:	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Friend";
}

CActionItem_MouseCmd_Friend::~CActionItem_MouseCmd_Friend()
{
	s_pMe = 0;
}

////////////////////////////////////////////////////////////////////////////////////
// CActionItem_MouseCmd_Exchange
////////////////////////////////////////////////////////////////////////////////////

CActionItem_MouseCmd_Exchange* CActionItem_MouseCmd_Exchange::s_pMe = 0;
CActionItem_MouseCmd_Exchange::CActionItem_MouseCmd_Exchange(INT id)
	:	CActionItem(id)
{
	s_pMe = this;
	m_strName = "MouseCmd_Exchange";
}

CActionItem_MouseCmd_Exchange::~CActionItem_MouseCmd_Exchange()
{
	s_pMe = 0;
}

////////////////////////////////////////////////////////////////////////////////////
// 
////////////////////////////////////////////////////////////////////////////////////
