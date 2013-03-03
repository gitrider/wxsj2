
#include "StdAfx.h"
#include "GMActionSystem_PetSkill.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIEventSystem.h"
#include "GIEventDefine.h"
#include "GIInputSystem.h"
#include "Interface\GMGameInterface.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDataPool.h"
#include "Event\GMEventSystem.h"
#include "DBC\GMDataBase.h"



CActionItem_PetSkill::CActionItem_PetSkill(INT nID) : CActionItem(nID)
{
	m_nSkillID = -1;
	m_nSkillIndex = -1;
}

CActionItem_PetSkill::~CActionItem_PetSkill()
{
}


VOID CActionItem_PetSkill::Update_PetSkill(const PET_GUID_t& PetGUID, const PET_SKILL* pPetSkill, INT nSkillIndex)
{
	if (NULL == pPetSkill->m_pDefine)
		KLThrow("Update_PetSkill(), pPetSkill->m_pDefine = NULL");

	m_GUID = PetGUID;

	m_nSkillIndex = nSkillIndex;

	m_nSkillID = pPetSkill->m_pDefine->m_nID;
	//名称
	m_strName = pPetSkill->m_pDefine->m_lpszName;
	//图标
	m_strIconName = pPetSkill->m_pDefine->m_lpszIconName;


	SetCheckState(FALSE);

	//通知UI
	UpdateToRefrence();
}
PET_SKILL* CActionItem_PetSkill::GetPetSkillImpl(VOID)
{
	if(m_nSkillIndex < 0 || m_nSkillIndex >= PET_MAX_SKILL_COUNT)
		return NULL;

	INT nIndex = CDataPool::GetMe()->Pet_GetPetByGuid(m_GUID);
	if (-1 == nIndex)
	{
		// 目标宠物技能		
		SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && pPet->m_GUID == m_GUID)
		{	
			return &(pPet->m_Skill[m_nSkillIndex]);
		}		
	}
	else
	{
		// 自己宠物技能	
		PET_SKILL* pSkill = CDataPool::GetMe()->Pet_GetSkill(nIndex, m_nSkillIndex);
		if (pSkill && pSkill->m_pDefine)
		{
			if (pSkill->m_pDefine->m_nID == m_nSkillID)
			{
				return pSkill;
			}			
		}
	}

	return NULL;
}

INT	CActionItem_PetSkill::GetDefineID(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (pSkill && pSkill->m_pDefine) 
	{
		return pSkill->m_pDefine->m_nID;
	}
	return INVALID_ID;
}

LPCTSTR CActionItem_PetSkill::GetDesc(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (pSkill && pSkill->m_pDefine) 
	{
		return pSkill->m_pDefine->m_lpszSkillExplainForInterface;
	}
	return NULL;
}

//得到冷却状组ID
INT	CActionItem_PetSkill::GetCoolDownID(VOID)
{
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (pSkill && pSkill->m_pDefine) 
	{
		return pSkill->m_pDefine->m_nCooldownID;
	}
	return INVALID_ID;
}

INT CActionItem_PetSkill::GetPosIndex(VOID)
{
	return m_nSkillIndex;
}

VOID CActionItem_PetSkill::DoAction(VOID)
{
	// 检查冷却是否结束
	if(!CoolDownIsOver()) 
		return;
	
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (NULL == pSkill || NULL == pSkill->m_pDefine) 
		return;

	// 坐骑状态
	if (0 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "技能需要坐骑");

		/*STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("技能需要坐骑");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());*/
		return;
	}

	// 是否为当前骑宠的技能
	if (m_GUID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "不是该坐骑的技能");

		/*STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("不是该坐骑的技能");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	*/
		return;
	}

	CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());

	//根据操作类型
	switch(pSkill->m_pDefine->m_nSelectType)
	{
	case SELECT_TYPE_NONE:	
	case SELECT_TYPE_CHARACTER:	
		{
			CDataPool::GetMe()->Pet_SendUseSkillMessage(pSkill->m_pDefine->m_nID, 
				((pTargetObj)?(pTargetObj->GetServerID()):(INVALID_ID)), -1, -1);

			//恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_POS:
		{
			CActionSystem::GetMe()->SetDefaultAction(this);
		}
		break;

	case SELECT_TYPE_SELF:
	default:
		{
			KLThrow("Invalid pet skill select type:%d", pSkill->m_pDefine->m_nSelectType);
		}
		break;
	}

}

BOOL CActionItem_PetSkill::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//对于没有冷却的Action
	if (INVALID_ID == nCoolDownID)
		return TRUE;

	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->PetSkillCoolDownGroup_Get(nCoolDownID);

	if(pCoolDownGroup && pCoolDownGroup->nTime <= 0) 
		return TRUE;

	return FALSE;
}

//拖动结束
VOID CActionItem_PetSkill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
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
				CActionSystem::GetMe()->MainMenuBar_Set(nIndex, INVALID_ID);
			}
			break;
		default:
			break;
		}
	}

	//拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0') 
		return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//如果不是拖到快捷栏，返回
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;

	INT nOldTargetId	= -1;
	INT nIndex			= -1;
	INT nPet_Num		= -1;

	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (NULL == pSkill || NULL == pSkill->m_pDefine) 
		return;

	switch(cTargetType)
	{
	case 'M': //主菜单
		{
			// 不能是被动技能
			if (pSkill->m_pDefine->m_nPassiveFlag)
				break;			

			// 不是当前宠物技能时，清除
			/*CActionItem_PetSkill* pPetSkillItem = (CActionItem_PetSkill*)pActionItem;
			if(pPetSkillItem->GetMountGUID() != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
			{
				MainMenuBar_Set(i,INVALID_ID);
				break;
			}*/

			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;

			// 查询目标位置原来的内容
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
			// 替换
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID());
		}
		break;

	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M': //主菜单
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10;
			// 交换
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, nOldTargetId);
		}
		break;
	default:
		break;
	} 
	//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}

VOID CActionItem_PetSkill::UpdateCoolDown(VOID)
{
	std::set< tActionReference* >::iterator it;
	INT nCoolDownID = GetCoolDownID();

	// 对于没有冷却的Action
	if(-1 == nCoolDownID)
	{
		for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		return;
	}

	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup = CDataPool::GetMe()->PetSkillCoolDownGroup_Get(nCoolDownID);
	if (pCoolDownGroup)
	{
		INT nTimeNow   = pCoolDownGroup->nTime;
		INT nTimeTotal = pCoolDownGroup->nTotalTime;

		for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
		{
			if(nTimeNow <= 0 || nTimeTotal <= 0)
			{
				(*it)->EnterCoolDown(-1, 0.0f);
			}
			else
			{
				(*it)->EnterCoolDown(nTimeNow, 1.0f - (FLOAT)nTimeNow/(FLOAT)nTimeTotal);
			}
		}
	}
}
