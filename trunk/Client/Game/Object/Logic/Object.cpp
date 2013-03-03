
#include "StdAfx.h"
#include <math.h>
#include "GIException.h"
#include "..\ObjectDef.h"
#include "Object.h"
#include "..\Manager\ObjectManager.h"
#include "Global.h"
#include "World\WorldManager.h"
#include "Procedure\GameProcedure.h"
#include "World\Path.h"
#include <limits>
//#include ".\Character\Obj_PlayerMySelf.h"



KL_IMPLEMENT_DYNAMIC(CObject, GETCLASS(tObject));



CObject::CObject()
{
	m_ID				= INVALID_UID;
	m_idServer			= INVALID_UID;
//	m_nLogicCount		= -1;
	m_nBaseLogicCount		= -1;
	m_nActionLogicCount		= -1;
	m_pRenderInterface	= NULL;
	m_fvPosition		= fVector3(0.0f, 0.0f, 0.0f);
	m_fvRotation		= fVector3(0.0f, 0.0f, 0.0f);
	m_dwStatusFlag		= OSF_VISIABLE;

	m_bFakeObject		= FALSE;

	SetMsgTime(::timeGetTime());
}

CObject::~CObject()
{
	KLAssert( m_pRenderInterface == NULL );
}


VOID CObject::Initial( VOID *pInit )
{
	const SObjectInit *pObjectInit = (SObjectInit*)pInit;
	if ( pObjectInit != NULL )
	{
		SetPosition( pObjectInit->m_fvPos );
		SetRotation( pObjectInit->m_fvRot );
	}

//	m_nLogicCount		= -1;
	m_nBaseLogicCount		= -1;
	m_nActionLogicCount		= -1;
}

VOID CObject::SetMapPosition(const fVector2& fvPosition)
{
	INT a = 0;
	//CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
	//KLAssert(pActiveScene);

	////---------------------------------------------------
	////����ȡ���ڵ����ϵĸ߶�
	//fVector3 fvAtTerrain;
	//CGameProcedure::s_pGfxSystem->Axis_Trans(
	//	tGfxSystem::AX_PLAN, fVector3(fvPosition.x, 0.0f, fvPosition.y),
	//	tGfxSystem::AX_GAME, fvAtTerrain);

	////---------------------------------------------------
	////ȡ�����������ϸ߶�
	//FLOAT fHeight = -FLT_MAX;
	//{
	//	if(!(pActiveScene->m_WalkCollisionMng.GetHeight(fvPosition.x, fvPosition.y, fHeight)))
	//	{
	//		fHeight = -FLT_MAX;
	//	}
	//}

	////---------------------------------------------------
	////�жϴ�ֵ
	//if(fHeight > fvAtTerrain.y)
	//	SetPosition(fVector3(fvPosition.x, fHeight, fvPosition.y));		//����������
	//else         
	//	SetPosition(fvAtTerrain);		//�ڵ�����

}

VOID CObject::SetPosition(const fVector3& fvPosition) 
{ 
	m_fvPosition = fvPosition;	
	CGameProcedure::m_bWaitNeedFreshMinimap = true;

	// ����Ⱦ��ͬ��λ��
	if(m_pRenderInterface)
		m_pRenderInterface->SetPosition(fvPosition);	// �������� �˷�	GetPosition()
}

VOID CObject::SetRotation(const fVector3& fvRotation) 
{ 
	m_fvRotation = fvRotation;	
	// ��Ϊ��Ķ�������Ҫ���з���ֻ����ҲŻ�
	CGameProcedure::m_bNeedFreshMinimap = true;
	
	// ����Ⱦ��ͬ��λ��
	if(m_pRenderInterface)
		m_pRenderInterface->SetOrientation(fvRotation);		// GetRotation()
}

VOID CObject::SetFootPosition(const fVector3& fvPosition) 
{ 
	m_fvFootPosition = fvPosition;	
}


VOID CObject::SetFaceDir(FLOAT fDir)	
{ 
	m_fvRotation.y = fDir;
	SetRotation( m_fvRotation );
}


VOID CObject::SetMsgTime(UINT dwTime)
{
	m_timeMsg = dwTime; 

	//�յ���Ϣ,�Զ�������Ұ
	//Disalbe(OSF_OUT_VISUAL_FIELD);
	//Enable(OSF_VISIABLE);
}

VOID CObject::Destroy(VOID)
{
	Release();
	CObjectManager::GetMe()->DestroyObject(this);
	return;
}

// ѹ��һ��ָ��
BOOL CObject::PushCommand(const SCommand_Object *pCmd )
{
	OnCommand( pCmd );
	return TRUE;
}

VOID CObject::Tick_CheckAlive(VOID)
{
	if(GetChildBuf().empty()) return;

	register std::list< tNode* >::iterator it;
	for(it=GetChildBuf().begin(); it!=GetChildBuf().end();)
	{
		//���ǿ�����Obj��ɾ��
		register std::list< tNode* >::iterator itNext = it;
		itNext++;
		
		((CObject*)(*it))->Tick_CheckAlive();

		it = itNext;
	}
}

VOID CObject::Enable(UINT dwFlag)
{
	m_dwStatusFlag |= dwFlag; 

	// ���Ӵ���
	switch(dwFlag)
	{
	case OSF_VISIABLE:
		if(m_pRenderInterface && !GetFakeObjectFlag() )m_pRenderInterface->SetVisible(true);
		break;

	case OSF_RAY_QUERY:
		if(m_pRenderInterface && !GetFakeObjectFlag() )m_pRenderInterface->SetRayQuery(true);
		break;

	default:
		break;
	}
}

VOID CObject::Disalbe(UINT dwFlag)
{ 
	m_dwStatusFlag &= (~dwFlag); 

	// ���Ӵ���
	switch(dwFlag)
	{
	case OSF_VISIABLE:
		{
			if( m_pRenderInterface )
				m_pRenderInterface->SetVisible(FALSE);

			if( this == CObjectManager::GetMe()->GetMainTarget() )
			{
				CObjectManager::GetMe()->SetMainTarget(INVALID_UID);
			}
		}
		break;

	case OSF_RAY_QUERY:
		if( m_pRenderInterface )
			m_pRenderInterface->SetRayQuery(FALSE);
		break;

	default:
		break;
	}
}

BOOL CObject::IsSpecialObject(STRING& strReturn)
{
	if(this == (CObject*)CObjectManager::GetMe()->GetMySelf())
	{
		strReturn = "player";
		return true;
	}
	else if(this == (CObject*)CObjectManager::GetMe()->GetMainTarget())
	{
		strReturn = "target";
		return true;
	}
	else
		return FALSE;
}

BOOL CObject::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	return TRUE;
}

BOOL CObject::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	return TRUE;
}

VOID CObject::PushDebugString(LPCTSTR szDebugString)
{
#ifdef ENABLE_OBJECT_DEBUG_STRING
	
	if(m_qDebugString.size() >= 16)
	{
		m_qDebugString.pop_front();
	}

	TCHAR szToPush[MAX_PATH] = {0};
	SYSTEMTIME timeNow;
	::GetLocalTime(&timeNow);
	_snprintf(szToPush, MAX_PATH, "%02d:%02d %s", timeNow.wMinute, timeNow.wSecond, szDebugString);

	STRING strTemp = szToPush;
	m_qDebugString.push_back(strTemp);

#endif
}
