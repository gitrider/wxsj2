
/**	GMActionSystem_Skill.cpp
 *	
 *	���ܣ�	���ܲ�������
 *	
 *	�޸ļ�¼��
 *			080225	���Ӽ���˲����ش���
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

	//����
	m_idSkillImpl =	pSkill->m_pDefine->m_nID;
	//����
	m_strName = pSkill->m_pDefine->m_lpszName;
	//ͼ��
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

		//֪ͨUI
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
		//��һ��������
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
		//��һ��������
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

//�õ���ȴ״��ID
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

	// �Ƿ��ܹ��ܹ��Զ�����
	if( pSkillImpl->m_pDefine->m_bAutoRedo )
	{
		CGameInterface::GetMe()->Skill_SetActive(this);
		return TRUE;
	}

	return FALSE;
}

VOID CActionItem_Skill::DoAction( VOID )
{
	// ��ȡ��ʹ�õļ���
	const SCLIENT_SKILL* pSkill = GetSkillImpl();
	if( !pSkill )
		return;

	INT idUser = -1;
	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if( !pMySelf )
		return; 

	idUser = pMySelf->GetID();	
	
	// �Զ����,����ʩ�ŵļ���
	if (TRUE == pSkill->m_pDefine->m_bAutoRedo)
	{
		tActionItem* pItem = CGameInterface::GetMe()->Skill_GetActive();
		// �м����
		if (pItem)
		{
			// ��ǰ�������ڼ�����
			if (pItem->GetID() == GetID())
			{
				// �˴β���Ϊ�رղ���
				CGameInterface::GetMe()->Skill_SetActive( CActionSystem::GetMe()->GetMyDefaultSkillAtion() );
				
				// �ر��Զ����
				CAI_MySelf* pMySelfAI = (CAI_MySelf*)( pMySelf->CharacterLogic_GetAI() );
				if( pMySelfAI )
					pMySelfAI->m_bCurrSkill  = FALSE;			

				return;
			}
			else
			{
				// ѡ��״̬
				AutoKeepOn();
			}
		}
		// û�м����
		else
		{
			// ѡ��״̬
			AutoKeepOn();
		}
	}
	// ����ʩ�ŵļ���
	else
	{
		// ˲�����ܲ������Զ�����
		if (FALSE == pSkill->m_pDefine->m_nImmediately)
		{
			CAI_MySelf* pMySelfAI = (CAI_MySelf*)( pMySelf->CharacterLogic_GetAI() );
			if( pMySelfAI )
				pMySelfAI->m_bCurrSkill  = FALSE;
		}		
	}

	// ���м���ʹ�õ������ж�
	// �Ƿ�ѧ�ᡢ��ȴʱ�䡢����
	{

		ORESULT oResult;
		// �Ƿ��Ѿ�ѧ��
		oResult = pSkill->IsCanUse_Leaned();
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

		// �����ȴ�Ƿ����
		oResult = pSkill->IsCanUse_CheckCoolDown();
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

		// ���ļ��
		oResult = pSkill->IsCanUse_CheckDeplete( idUser );
		if( OR_FAILED( oResult ) )
		{
			CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
			return ;
		}

	}


	//================
	// �������ѡ���������
	switch( pSkill->m_pDefine->m_nSelectType )	// �����ʹӱ��ж���
	{
	case SELECT_TYPE_NONE:	// ����ѡ��,ֱ��ʩ�ţ����Լ�Ϊ���ĵķ�Χ�Զ�������
		{
			// id��λ�ã�����
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_CHARACTER:	// ѡ���ɫ
		{
			// ����ѡ����Ŀ��
			if( CObjectManager::GetMe()->GetMainTarget() )
			{
				// id��Ŀ��id
				CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), CObjectManager::GetMe()->GetMainTarget()->GetServerID());
			}
			else
			{
				// ��ѡ��Ŀ����ʾ��Ϣ
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( OR_NO_TARGET ) );
			}

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_POS:	// ������λ��Ϊ���ĵķ�Χ����
		{
			// id��
			//CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(this);
		}
		break;

	case SELECT_TYPE_DIR:	// �������Σ�

		// ����ķ�����ʹ��
		//CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), fVector2(pMySelf->GetPosition().x, pMySelf->GetPosition().z), pMySelf->GetFaceDir());

		CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		break;

	case SELECT_TYPE_SELF:	// ���Լ����в���	
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), CObjectManager::GetMe()->GetMySelf()->GetServerID());
			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SELECT_TYPE_HUMAN_GUID:	// �������
		{
			// ��ǰ�Ƿ��Ѿ�ѡ����һ������
			GUID_t guid;
			CObjectManager::GetMe()->GetMainTarget(guid);
			if(guid == (GUID_t)-1)
			{
				// ��δѡ��Ϸ��Ķ���
				STRING strTemp = NOCOLORMSGFUNC("GMDP_Struct_Skill_Info_Invalidate_Target");
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, strTemp.c_str());
				return;
			}

			CGameProcedure::s_pGameInterface->Player_UseSkill(GetID(), (GUID_t)guid);
			// �ָ�����Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;
	}
}

BOOL CActionItem_Skill::CoolDownIsOver(VOID)
{
	INT nCoolDownID = GetCoolDownID();

	//����û����ȴ��Action
	if(INVALID_ID == nCoolDownID) return TRUE;
	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->CoolDownGroup_Get(nCoolDownID);

	if( pCoolDownGroup && pCoolDownGroup->nTime <= 0 && CDataPool::GetMe()->CommonCoolDown_Get() <= 0 ) 
		return TRUE;
	else 
		return FALSE;
}

//�϶�����
VOID CActionItem_Skill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
{
	if( bDestory )
	{
		CHAR cSourceType = szSourceName[0];
		switch(cSourceType)
		{
		case 'M':		//���˵�
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

	// �϶����հ׵ط�
	if(!szTargetName || szTargetName[0]=='\0') return;
	
	// ���������disable״̬�������϶�
	if(!IsEnabled()) return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	// ��������ϵ������������
	if( cSourceName == 'M' && cTargetType != 'M' )
		return;

	INT nOldTargetId = -1;
	INT nIndex =-1;

	switch(cTargetType)
	{
	case 'M':		//���˵�
		{
			if(IsPassiveSkill()) return;
			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;
			//��ѯĿ��λ��ԭ��������
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

// �õ����ܵȼ�
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

// �õ����ܵȼ�
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

// �õ��Ƿ�ѧϰ�˼���
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