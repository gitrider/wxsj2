
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
	//����
	m_strName = pPetSkill->m_pDefine->m_lpszName;
	//ͼ��
	m_strIconName = pPetSkill->m_pDefine->m_lpszIconName;


	SetCheckState(FALSE);

	//֪ͨUI
	UpdateToRefrence();
}
PET_SKILL* CActionItem_PetSkill::GetPetSkillImpl(VOID)
{
	if(m_nSkillIndex < 0 || m_nSkillIndex >= PET_MAX_SKILL_COUNT)
		return NULL;

	INT nIndex = CDataPool::GetMe()->Pet_GetPetByGuid(m_GUID);
	if (-1 == nIndex)
	{
		// Ŀ����＼��		
		SDATA_PET* pPet = CDataPool::GetMe()->TargetPet_GetPet();
		if (pPet && pPet->m_GUID == m_GUID)
		{	
			return &(pPet->m_Skill[m_nSkillIndex]);
		}		
	}
	else
	{
		// �Լ����＼��	
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

//�õ���ȴ״��ID
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
	// �����ȴ�Ƿ����
	if(!CoolDownIsOver()) 
		return;
	
	const PET_SKILL* pSkill = GetPetSkillImpl();
	if (NULL == pSkill || NULL == pSkill->m_pDefine) 
		return;

	// ����״̬
	if (0 > CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MountID())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "������Ҫ����");

		/*STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("������Ҫ����");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());*/
		return;
	}

	// �Ƿ�Ϊ��ǰ���ļ���
	if (m_GUID != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "���Ǹ�����ļ���");

		/*STRING strTemp = "";
		strTemp = NOCOLORMSGFUNC("���Ǹ�����ļ���");
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, strTemp.c_str());	*/
		return;
	}

	CObject *pTargetObj = (CObject*)(CObjectManager::GetMe()->GetMainTarget());

	//���ݲ�������
	switch(pSkill->m_pDefine->m_nSelectType)
	{
	case SELECT_TYPE_NONE:	
	case SELECT_TYPE_CHARACTER:	
		{
			CDataPool::GetMe()->Pet_SendUseSkillMessage(pSkill->m_pDefine->m_nID, 
				((pTargetObj)?(pTargetObj->GetServerID()):(INVALID_ID)), -1, -1);

			//�ָ�����Action
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

	//����û����ȴ��Action
	if (INVALID_ID == nCoolDownID)
		return TRUE;

	//ȡ����ȴ������
	const CDataPool::COOLDOWN_GROUP* pCoolDownGroup =
		CDataPool::GetMe()->PetSkillCoolDownGroup_Get(nCoolDownID);

	if(pCoolDownGroup && pCoolDownGroup->nTime <= 0) 
		return TRUE;

	return FALSE;
}

//�϶�����
VOID CActionItem_PetSkill::NotifyDragDropDragged(BOOL bDestory, LPCSTR szTargetName, LPCSTR szSourceName)
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
				CActionSystem::GetMe()->MainMenuBar_Set(nIndex, INVALID_ID);
			}
			break;
		default:
			break;
		}
	}

	//�϶����հ׵ط�
	if(!szTargetName || szTargetName[0]=='\0') 
		return;

	CHAR cSourceName = szSourceName[0];
	CHAR cTargetType = szTargetName[0];

	//��������ϵ������������
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
	case 'M': //���˵�
		{
			// �����Ǳ�������
			if (pSkill->m_pDefine->m_nPassiveFlag)
				break;			

			// ���ǵ�ǰ���＼��ʱ�����
			/*CActionItem_PetSkill* pPetSkillItem = (CActionItem_PetSkill*)pActionItem;
			if(pPetSkillItem->GetMountGUID() != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID() )
			{
				MainMenuBar_Set(i,INVALID_ID);
				break;
			}*/

			INT nIndex = szTargetName[1]-'0';
			nIndex = szTargetName[2]-'0' + nIndex*10 ;

			// ��ѯĿ��λ��ԭ��������
			nOldTargetId = CActionSystem::GetMe()->MainMenuBar_Get(nIndex);
			// �滻
			CActionSystem::GetMe()->MainMenuBar_Set(nIndex, GetID());
		}
		break;

	default:
		break;
	}

	switch(cSourceName)
	{
	case 'M': //���˵�
		{
			INT nIndex = szSourceName[1]-'0';
			nIndex = szSourceName[2]-'0' + nIndex*10;
			// ����
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

	// ����û����ȴ��Action
	if(-1 == nCoolDownID)
	{
		for(it=m_setReference.begin(); it!=m_setReference.end(); it++)
		{
			(*it)->EnterCoolDown(-1, 0.0f);
		}
		return;
	}

	//ȡ����ȴ������
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
