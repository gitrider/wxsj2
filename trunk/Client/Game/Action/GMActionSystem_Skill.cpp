
/**	GMActionSystem_Skill.cpp
 *	
 *	功能：	技能操作处理
 *	
 *	修改记录：
 *			080225	增加技能瞬发相关代码
 *			
 */

#include "StdAfx.h"
#include "GMActionSystem_Skill.h"
#include "Procedure\GameProcedure.h"
#include "DataPool\GMDP_CharacterData.h"
#include "GIEventSystem.h"
#include "GIEventDefine.h"
#include "Interface\GMGameInterface.h"
#include "Object\Manager\ObjectManager.h"
#include "Object\Logic\Character\Obj_PlayerMySelf.h"
#include "DataPool\GMDataPool.h"
#include "Interface\GMInterface_Script_Tooltip.h"
#include "Event\GMEventSystem.h"
#include "Object\Logic\Character\ai\GMAI_MySelf.h"
#include "..\..\..\Common\Utilities\SplitStringLevelOne.h"


CActionItem_Skill::CActionItem_Skill(INT nID) : CActionItem(nID)
{
}

CActionItem_Skill::~CActionItem_Skill()
{
}


VOID CActionItem_Skill::Update_Skill(const SCLIENT_SKILL* pSkill)
{
	KLAssert(pSkill);

	//引用
	m_idSkillImpl =	pSkill->m_pDefine->m_nID;
	//名称
	m_strName = pSkill->m_pDefine->m_lpszName;
	//图标
	m_strIconName = pSkill->m_pDefine->m_lpszIconName;

	BOOL bOldEnable = IsEnabled();
	BOOL bNewEnable = FALSE;
	ORESULT oResult = pSkill->IsCanUse_CheckDeplete( CObjectManager::GetMe()->GetMySelf()->GetID());
	if ( OR_SUCCEEDED( oResult ) )
	{
		oResult = pSkill->IsCanUse_Leaned();
		if ( OR_SUCCEEDED( oResult ) )
		{
			bNewEnable = TRUE;
		}
	}

	if ( bNewEnable != bOldEnable )
	{
		(bNewEnable)?(Enable()):(Disable());

		//通知UI
		UpdateToRefrence();
	}
}

const SCLIENT_SKILL* CActionItem_Skill::GetSkillImpl( VOID )
{
	return CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( m_idSkillImpl );
}

INT	CActionItem_Skill::GetDefineID(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();	
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nID;
}

LPCTSTR CActionItem_Skill::GetDesc(VOID)
{
	SCLIENT_SKILL* pSkill = const_cast<SCLIENT_SKILL*>(GetSkillImpl());

	if( pSkill )
	{
		//第一次做分析
		if(m_strTipArray.size() == 0)
		{
			SplitStringLevelOne pSplitL1;
			pSplitL1.Reset();
			pSplitL1.Init( '|', NULL );
			pSplitL1.DoSplit( pSkill->GetSkillDesc() );
			INT iLineCount	= pSplitL1.GetResultLineCount();

			m_strTipArray.resize(iLineCount+1);

			for( INT nID = 0; nID< iLineCount; ++nID )
			{
				m_strTipArray[nID] = pSplitL1.GetResultLine(nID);
			}
		}
		if(pSkill->m_nLevel >0 && m_strTipArray.size() >= pSkill->m_nLevel)
		{
			return m_strTipArray[pSkill->m_nLevel-1].c_str();
		}
		return pSkill->GetSkillDesc();
	}//

	return "";
}

LPCTSTR CActionItem_Skill::GetNextLevelDesc(VOID)
{
	SCLIENT_SKILL* pSkill = const_cast<SCLIENT_SKILL*>(GetSkillImpl());

	if( pSkill )
	{
		//第一次做分析
		if(m_strTipArray.size() == 0)
		{
			SplitStringLevelOne pSplitL1;
			pSplitL1.Reset();
			pSplitL1.Init( '|', NULL );
			pSplitL1.DoSplit( pSkill->GetSkillDesc() );
			INT iLineCount	= pSplitL1.GetResultLineCount();

			m_strTipArray.resize(iLineCount+1);

			for( INT nID = 0; nID< iLineCount; ++nID )
			{
				m_strTipArray[nID] = pSplitL1.GetResultLine(nID);
			}
		}
		if(pSkill->m_nLevel >=0 && m_strTipArray.size() > pSkill->m_nLevel)
		{
			return m_strTipArray[pSkill->m_nLevel].c_str();
		}
		return pSkill->GetSkillDesc();
	}//

	return "";
}

//得到冷却状组ID
INT	CActionItem_Skill::GetCoolDownID(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if(!pSkill) return -1;

	return pSkill->m_pDefine->m_nCooldownID;
}

INT CActionItem_Skill::GetPosIndex(VOID)
{
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if(!pSkill) return -1;

	return pSkill->m_nPosIndex;
}

BOOL CActionItem_Skill::AutoKeepOn(VOID)
{
	const SCLIENT_SKILL* pSkillImpl = GetSkillImpl();
	if( !pSkillImpl )
		return FALSE;

	// 是否能够能够自动进行
	if( pSkillImpl->m_pDefine->m_bAutoRedo )
	{
		CGameInterface::GetMe()->Skill_SetActive(this);
		return TRUE;
	}

	return FALSE;
}

VOID CActionItem_Skill::DoAction( VOID )
{
	// 获取将使用的技能
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if( !pSkill )
		return;

	INT idUser = -1;
	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if( !pMySelf )
		return; 

	idUser = pMySelf->GetID();	
	
	// 自动打怪,连续施放的技能
	if (TRUE == pSkill->m_pDefine->m_bAutoRedo)
	{
		tActionItem* pItem = CGameInterface::GetMe()->Skill_GetActive();
		// 有激活技能
		if (pItem)
		{
			// 当前技能正在激活中
			if (pItem->GetID() == GetID())
			{
				// 此次操作为关闭操作
				CGameInterface::GetMe()->Skill_SetActive( CActionSystem::GetMe()->GetMyDefaultSkillAtion() );
				
				// 关闭自动打怪
				CAI_MySelf* pMySelfAI = (CAI_MySelf*)( pMySelf->CharacterLogic_GetAI() );
				if( pMySelfAI )
					pMySelfAI->m_bCurrSkill  = FALSE;			

				return;
			}
			else
			{
				// 选中状态
				AutoKeepOn();
			}
		}
		// 没有激活技能
		else
		{
			// 选中状态
			AutoKeepOn();
		}
	}
	// 单次施放的技能
	else
	{
		// 瞬发技能不会打断自动技能
		if (FALSE == pSkill->m_pDefine->m_nImmediately)
		{
			CAI_MySelf* pMySelfAI = (CAI_MySelf*)( pMySelf->CharacterLogic_GetAI() );
			if( pMySelfAI )
				pMySelfAI->m_bCurrSkill  = FALSE;
		}		
	}

	// 进行技能使用的条件判断
	// 是否学会、冷却时间、消耗
	{

		ORESULT oResult;
		// 是否已经学会
		oResult = pSkill->IsCanUse_Leaned();
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

		// 检查冷却是否结束
		oResult = pSkill->IsCanUse_CheckCoolDown();
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

		// 消耗检测
		oResult = pSkill->IsCanUse_CheckDeplete( idUser );
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

	}


	//================
	// 根据鼠标选择操作类型
	switch( pSkill->m_pDefine->m_nSelectType )	// 此类型从表中读出
	{
	case SELECT_TYPE_NONE:	// 无需选择,直接施放（以自己为中心的范围自动攻击）
		{
			// id，位置，方向
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_CHARACTER:	// 选择角色
		{
			// 有无选择主目标
			if( CObjectManager::GetMe()->GetMainTarget() )
			{
				// id，目标id
				CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), CObjectManager::GetMe()->GetMainTarget()->GetServerID());
			}
			else
			{
				// 无选择目标提示信息
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_NO_TARGET ) );
			}

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_POS:	// 以鼠标的位置为中心的范围攻击
		{
			// id，
			//CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(this);
		}
		break;

	case SELECT_TYPE_DIR:	// 方向（扇形）

		// 人物的方向技能使用
		//CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

		CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		break;

	case SELECT_TYPE_SELF:	// 对自己进行操作	
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), CObjectManager::GetMe()->GetMySelf()->GetServerID());
			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_HUMAN_GUID:	// 其他玩家
		{
			// 当前是否已经选择了一个队友
			GUID_t guid;
			CObjectManager::GetMe()->GetMainTarget(guid);
			if(guid == (GUID_t)-1)
			{
				// 尚未选择合法的对象
				STRING strTemp = NOCOLORMSGFUNC("GMDP_Struct_Skill_Info_Invalidate_Target");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}

			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), (GUID_t)guid);
			// 恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	}
}

BOOL CActionItem_Skill::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//对于没有冷却的Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//取得冷却组数据
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//拖动结束
VOID CActionItem_Skill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
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

	// 拖动到空白地方
	if(!szTargetName || szTargetName[0]=='\0') return;
	
	// 如果技能是disable状态，不能拖动
	if(!IsEnabled()) return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	// 如果不是拖到快捷栏，返回
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;

	INT nOldTargetId = -1;
	INT nIndex =-1;

	switch(cTargetType)
	{
	case 'M':		//主菜单
		{
			if(IsPassiveSkill()) return;
			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;
			//查询目标位置原来的内容
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
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

//	CEventSystem::GetMe()->PushEvent(GE_UPDATE_SUPERTOOLTIP);
	CEventSystem::GetMe()->PushEvent(GE_SUPERTOOLTIP, "0");
}

// 得到技能等级
INT	CActionItem_Skill::GetSkillLevel()
{
	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
	if(pImpl)
	{
		return pImpl->m_nLevel;
	}
	return -1;
}

INT	CActionItem_Skill::GetCooldownTime()
{
	SCLIENT_SKILL* pSkill = const_cast<SCLIENT_SKILL*>(GetSkillImpl());

	if( pSkill )
	{
		if( m_nCooldownTime.size() == 0 )
		{
			SplitStringLevelOne pSplitL1;
			pSplitL1.Reset();
			pSplitL1.Init( '|', NULL );
			pSplitL1.DoSplit( pSkill->GetSkillData()->m_lpsznCoolDownTime);
			INT iLineCount	= pSplitL1.GetResultLineCount();

			m_nCooldownTime.resize(iLineCount+1);

			for( INT nID = 0; nID< iLineCount; ++nID )
			{
				m_nCooldownTime[nID] = atoi( pSplitL1.GetResultLine(nID) );
			}
		}

		if(pSkill->m_nLevel >0 && m_nCooldownTime.size() >= pSkill->m_nLevel)
		{
			return m_nCooldownTime[pSkill->m_nLevel-1];
		}
		if (pSkill->m_nLevel == 0)
		{
			return m_nCooldownTime[0];
		}
	}
	return 0;
}

// 得到技能等级
INT	CActionItem_Skill::GetSkillXinfaLevel()
{
//	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
//	if( pImpl )
//	{
//		INT id = 0;
//		
//		if(NULL == pImpl->m_pDefine)
//		{
//			return -1;
//		}
//
//
//		const SCLIENT_XINFA* pXinfa = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_XinFa(pImpl->m_pDefine->m_nXinFaID);
//		if(!pXinfa) 
//		{
//			return -1;
//		};
//
//		return pXinfa->m_nLevel;
//		
//	}
//
	return -1;
}

// 得到是否学习了技能
BOOL CActionItem_Skill::GetIsLearnedSkill()
{
	const SCLIENT_SKILL* pImpl = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(m_idSkillImpl);
	if( pImpl )
	{
		return pImpl->m_bLeaned;
	}

	return FALSE;

}
BOOL	CActionItem_Skill::IsPassiveSkill(VOID)
{
	SCLIENT_SKILL * pSkillDefine =(SCLIENT_SKILL*)GetSkillImpl();
	if (pSkillDefine && pSkillDefine->m_pDefine)
	{
		return pSkillDefine->m_pDefine->m_nPassiveFlag;
	}
	return FALSE;
}