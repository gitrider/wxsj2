
#include "StdAfx.h"
#include "Obj_Item_Task.h"
#include "GIException.h"
#include "..\character\obj_playermyself.h"
#include "DataPool\GMDataPool.h"
#include "DataPool\GMDP_CharacterData.h"
#include "..\..\Manager\ObjectManager.h"
#include "Procedure\GameProcedure.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "GIGameInterface.h"



CObject_Item_Task::CObject_Item_Task(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_Task::~CObject_Item_Task()
{
}


VOID CObject_Item_Task::AsTask(const _DBC_ITEM_TASK* pTaskDefine)
{
	KLAssert(pTaskDefine);

	m_theBaseDef = pTaskDefine;
	m_nParticularID = ( ( m_theBaseDef->nClass * 100 + m_theBaseDef->nType ) * 10000 ) + m_theBaseDef->nIndex;

}

//�õ�ͼ��
LPCTSTR  CObject_Item_Task::GetIconName(VOID) const
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szIcon;
	}
	return NULL;
	
}

//��������

LPCTSTR		CObject_Item_Task::GetDropVisualID(VOID)	const
{
	
	return NULL;
}


//������ɫ
 

LPCTSTR		CObject_Item_Task::GetDropVisColor(VOID)	const
{

	return NULL;
}

//������ϸ����
VOID CObject_Item_Task::SetExtraInfo(const _ITEM * pItemInfo)
{
	KLAssert(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());
	for ( int i=0; i<MAX_ITEM_PARAM; ++i)
	{
		m_Param[i] = pItemInfo->m_Param[i];
	}
}

LPCTSTR  CObject_Item_Task::GetExtraDesc(VOID)
{
	if (m_theBaseDef)
	{
		m_strExtraDesc = m_theBaseDef->szDesc;
		return m_strExtraDesc.c_str();
	}
	return NULL;
}

//��¡��ϸ��Ϣ
VOID	CObject_Item_Task::Clone(const CObject_Item * pItemSource)
{

	SetNumber(((CObject_Item_Task*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

//�õ���Ʒ�����
ITEM_CLASS	CObject_Item_Task::GetItemClass(VOID) const 
{
	if (m_theBaseDef)
	{
		return (ITEM_CLASS)m_theBaseDef->nClass; 
	}
	return ITEM_CLASS(-1);
}
// �õ���Ʒ�Ƿ�㲥

BOOL     CObject_Item_Task::GetBroadcast(VOID)const 
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nBroadcast);
	}

	return FALSE;
}

	
// �õ���Ʒ����ļ۸�
INT		CObject_Item_Task::GetItemBasePrice()
{
	if(m_theBaseDef)
	{
		// 20100408 ModifyCodeBegin
		// return (m_theBaseDef->nBasePrice * GetNumber());
		return (m_theBaseDef->nBasePrice );
		// 20100408 ModifyCodeEnd
	}
	return 0;
}


// �õ���Ʒ����npc�ļ۸�
INT		CObject_Item_Task::GetItemPrice()
{
	if(m_theBaseDef)
	{
		// 20100408 ModifyCodeBegin
		// 	return (m_theBaseDef->nSalePrice * GetNumber());
		return (m_theBaseDef->nSalePrice);
		// 20100408 ModifyCodeEnd		
	}
	return -1;

}

// �õ���Ʒ������ļ���ID
INT		CObject_Item_Task::GetItemSkillID()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nSkillID;
	}
	return -1;
}

INT		CObject_Item_Task::GetItemTargetType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nTargetType;
	}
	return ITEM_TARGET_TYPE_INVALID;
}
//��Ҫ�ȼ�
INT		CObject_Item_Task::GetNeedLevel()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nLevelRequire;
	}
	return -1;
}

// �õ�����Ʒ�ڱ��е�����
INT CObject_Item_Task::GetTaskItemTableType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}

	return -1;
}


// �õ�type��Ϣ
INT	CObject_Item_Task::GetItemTableType(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}
 return  -1;
}


bool CObject_Item_Task::IsAreaTargetType(void) const
{
	if(!m_theBaseDef) return false;
	//����Լ�
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//����Ŀ��
	return (ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType);
}

bool CObject_Item_Task::IsTargetOne(void) const
{
	if(!m_theBaseDef) return false;

	return (ITEM_TARGET_TYPE_SELF==m_theBaseDef->nTargetType || 
			ITEM_TARGET_TYPE_SELF_PET==m_theBaseDef->nTargetType);
}


	// �õ���Ʒ���������� 
LPCTSTR	CObject_Item_Task::GetItemTableTypeDesc()
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->szTypeDesc;
	}
	return NULL;
	
}


INT CObject_Item_Task::GetLayNum(VOID)
{
	if (m_theBaseDef)
	{
		return m_theBaseDef->nPileCount;
	}
	return 1;
}

INT CObject_Item_Task::WriteVarAttr(CHAR *pIn)
{
	Assert( pIn );

	CHAR* pBuff = pIn;
	pBuff += sizeof( UINT );


	return (INT)(pBuff-pIn);
}

BOOL CObject_Item_Task::ReadVarAttr(CHAR* pOut, INT& OutLength )
{

	return true;
}