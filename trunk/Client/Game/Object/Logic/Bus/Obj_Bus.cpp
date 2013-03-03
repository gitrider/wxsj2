#include "StdAfx.h"

#include "GIUtil.h"
#include "GIException.h"
#include "GIDBC_Struct.h"
#include "GITimeSystem.h"
#include "..\..\..\Dbc\GMDataBase.h"
#include "..\..\..\Procedure\GameProcedure.h"
#include "..\..\Manager\ObjectManager.h"
#include "..\..\ObjectCommandDef.h"
#include "..\Character\Obj_Character.h"
#include "..\..\..\Global.h"
#include "..\..\Manager\ActionSetManager.h"
#include "..\..\..\Interface\GMInterface_Script_Talk.h"
#include "..\..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\..\Variable\GMVariable.h"

#include "Obj_Bus.h"

#define OUTMSG(x)				{ADDTALKMSG(x);OutputDebugString(x);OutputDebugString("\n");}

KL_IMPLEMENT_DYNAMIC(CObject_Bus, GETCLASS(CObject_Dynamic));

CObject_Bus::CObject_Bus()
{
	m_nDataID			= INVALID_ID;
	m_nPassengerCount	= 0;
	INT i;
	for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
	{
		m_anPassengerIDs[i] = INVALID_ID;
		m_abPassengerAttacheds[i] = FALSE;
	}

	m_nCurrentAnimationIndex = -1;

	m_pBusData			= NULL;

	m_fvMoveTargetPos	= fVector3(-1.f, -1.f, -1.f);
	m_bMoving			= FALSE;
}

CObject_Bus::~CObject_Bus()
{
}

VOID CObject_Bus::Initial(VOID* pParam)
{
	CObject_Dynamic::Initial(pParam);

	RemoveAllPassengers();

	m_fvMoveTargetPos	= fVector3(-1.f, -1.f, -1.f);
	m_bMoving			= FALSE;

	m_pBusData			= NULL;
	m_nCurrentAnimationIndex = -1;

	SObject_BusInit *pBusInit = (SObject_BusInit*)pParam;
	if(pBusInit != NULL)
	{
		m_nDataID			= pBusInit->m_nDataID;
		INT i;
		for(i = 0; i < pBusInit->m_nPassengerCount; i++)
		{
			AddPassenger(i, pBusInit->m_anPassengerIDs[i]);
		}
	}
	else
	{
		m_nDataID			= INVALID_ID;
	}
	UpdateData();

	UpdateAnimation();
}

VOID CObject_Bus::Release(VOID)
{
	RemoveAllPassengers();
	ReleaseRenderInterface();
	m_nDataID			= INVALID_ID;
	m_pBusData			= NULL;
	m_fvMoveTargetPos	= fVector3(-1.f, -1.f, -1.f);
	m_bMoving			= FALSE;
	m_nCurrentAnimationIndex = -1;
	CObject_Dynamic::Release();
}

VOID CObject_Bus::Tick(VOID)
{
	CObject_Dynamic::Tick();

	if(m_bMoving)
	{
		UINT uElapseTime = (UINT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
		Tick_Move(uElapseTime);
	}

	UpdatePassengerAttached();
}

VOID CObject_Bus::SetPosition(const fVector3& fvPosition) 
{

	CObject_Dynamic::SetPosition( fvPosition );

	INT i;
	for(i = 0; i < m_nPassengerCount; i++)
	{
		if(m_anPassengerIDs[i] != INVALID_ID)
		{
			CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
			if(pObj != NULL)
			{
				pObj->SetMapPosition(fVector2(fvPosition.x, fvPosition.z));
				pObj->SetFootPosition(fvPosition);
			}
		}
	}
}

VOID CObject_Bus::SetRotation(const fVector3& fvRotation) 
{ 
	m_fvRotation = fvRotation;	
	CObject_Dynamic::SetRotation( fvRotation );

	INT i;
	for(i = 0; i < m_nPassengerCount; i++)
	{
		if(m_anPassengerIDs[i] != INVALID_ID)
		{
			CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
			if(pObj != NULL)
			{
				pObj->SetRotation(fvRotation);
			}
		}
	}
}

// 压入一条指令
BOOL CObject_Bus::PushCommand(const SCommand_Object *pCmd)
{
	switch(pCmd->m_wID)
	{
	case OC_BUS_MOVE:
		{
			FLOAT x = pCmd->m_afParam[0];
			FLOAT y = pCmd->m_afParam[1];
			FLOAT z = pCmd->m_afParam[2];

			fVector3 fvGame(x, y, z);
			StartMove(&fvGame);
		}
		break;
	case OC_BUS_STOP_MOVE:
		{
			FLOAT x = pCmd->m_afParam[0];
			FLOAT y = pCmd->m_afParam[1];
			FLOAT z = pCmd->m_afParam[2];

			fVector3 fvGame;
			CGameProcedure::s_pGfxSystem->Axis_Trans(
				tGfxSystem::AX_PLAN, fVector3(x, 0.0f, z),
				tGfxSystem::AX_GAME, fvGame);

			SetPosition(fvGame);
			StopMove();
		}
		break;
	case OC_BUS_ADD_PASSENGER:
		{
			INT nIndex			= pCmd->m_anParam[0];
			INT nPassengerID	= pCmd->m_anParam[1];

			AddPassenger(nIndex, nPassengerID);
		}
		break;
	case OC_BUS_REMOVE_PASSENGER:
		{
			INT nPassengerID	= pCmd->m_anParam[0];

			RemovePassenger(nPassengerID);
		}
		break;
	case OC_BUS_REMOVE_ALL_PASSENGER:
		{
			RemoveAllPassengers();
		}
		break;
	default:
		break;
	}
	return TRUE;
}

BOOL CObject_Bus::StartMove(const fVector3 *pTargetPos)
{
	if(pTargetPos == NULL)
	{
		return FALSE;
	}

	m_fvMoveTargetPos = *pTargetPos;
	m_fvSaveStartPos = GetPosition();

	m_fTargetDir = KLU_GetYAngle(fVector2(GetPosition().x, GetPosition().z), fVector2(m_fvMoveTargetPos.x, m_fvMoveTargetPos.z));
	if(m_fTargetDir < 0.f)
	{
		m_fTargetDir = __PI * 2.f + m_fTargetDir;
	}

	if(m_nCurrentAnimationIndex != BASE_ACTION_N_RUN)
	{
		ChangeAction(BASE_ACTION_N_RUN, 1.f, TRUE);
	}

	m_bMoving = TRUE;
	return TRUE;
}

VOID CObject_Bus::StopMove(VOID)
{
	m_bMoving = FALSE;

	ChangeActionLoop(FALSE);
	//ChangeAction(BASE_ACTION_N_IDLE, 1.f, TRUE);
}

BOOL CObject_Bus::Tick_Move(UINT uElapseTime)
{
	FLOAT fElapseTime = (FLOAT)uElapseTime / 1000.f;
	FLOAT fMoveDist = fElapseTime * GetMoveSpeed();
	FLOAT fDist = KLU_GetDist(fVector3(GetPosition().x, 0.f, GetPosition().z), fVector3(m_fvMoveTargetPos.x, 0.f, m_fvMoveTargetPos.z));
	if(fMoveDist >= fDist)
	{
		if(m_fvMoveTargetPos.y <= DEF_BUS_PATH_NODE_INVALID_Y)
		{
			SetMapPosition(fVector2(m_fvMoveTargetPos.x, m_fvMoveTargetPos.z));
		}
		else
		{
			SetPosition(m_fvMoveTargetPos);
		}
		StopMove();
	}
	else
	{
		if(fDist > 0.f)
		{
			fVector3 fvThisPos = GetPosition();
			fVector3 fvLen = m_fvMoveTargetPos - fvThisPos;
			if(m_fvMoveTargetPos.y <= DEF_BUS_PATH_NODE_INVALID_Y)
			{
				fvThisPos.x += (fvLen.x/fDist) * fMoveDist;
				fvThisPos.z += (fvLen.z/fDist) * fMoveDist;
				SetMapPosition(fVector2(fvThisPos.x, fvThisPos.z));
			}
			else
			{
				fvThisPos.x += (fvLen.x/fDist) * fMoveDist;
				fvThisPos.y += (fvLen.y/fDist) * fMoveDist;
				fvThisPos.z += (fvLen.z/fDist) * fMoveDist;
				SetPosition(fvThisPos);
			}
		}
	}

	FLOAT fFaceDir = GetFaceDir();
	if(fabsf(m_fTargetDir - fFaceDir) > 0.01f)
	{
		if(fabsf(m_fTargetDir - fFaceDir) < __PI * 5.f / 4.f
			&& fabsf(m_fTargetDir - fFaceDir) > __PI * 3.f / 4.f)
		{
			SetFaceDir(m_fTargetDir);
		}
		else
		{
			FLOAT fElapseDir = fElapseTime * __PI;
			if(m_fTargetDir - fFaceDir > __PI
				|| (m_fTargetDir - fFaceDir < 0.f && m_fTargetDir - fFaceDir > -__PI))
			{// 正转
				fFaceDir -= fElapseDir;
				if(fFaceDir < 0.f)
				{
					fFaceDir += 2.f *__PI;
				}
				SetFaceDir(fFaceDir);
			}
			else
			{// 反转
				fFaceDir += fElapseDir;
				if(fFaceDir > m_fTargetDir)
				{
					fFaceDir = m_fTargetDir;
				}
				SetFaceDir(fFaceDir);
			}
		}
	}
	return TRUE;
}

INT CObject_Bus::GetMaxPassengerCount(VOID)const
{
	if(m_pBusData != NULL)
	{
		return m_pBusData->m_nMaxPassengerCount;
	}
	return 0;
}

FLOAT CObject_Bus::GetMoveSpeed(VOID)const
{
	if(m_pBusData != NULL)
	{
		return m_pBusData->m_fMoveSpeed;
	}
	return 0.f;
}

BOOL CObject_Bus::IsChauffeur(ObjID_t nObjID)const
{
	return FALSE;
}

BOOL CObject_Bus::IsPassenger(ObjID_t nObjID)const
{
	if(nObjID == INVALID_ID)
		return FALSE;

	INT i;
	for(i = 0; i < m_nPassengerCount; i++)
	{
		if(nObjID == m_anPassengerIDs[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}

INT CObject_Bus::GetPassengerActionSetFileIndex(ObjID_t nObjID)const
{
	if(nObjID == INVALID_ID)
		return -1;

	if(m_pBusData == NULL)
		return -1;

	if(IsChauffeur(nObjID))
	{
	}
	else
	{
		INT i;
		for(i = 0; i < m_nPassengerCount; i++)
		{
			if(nObjID == m_anPassengerIDs[i])
			{
				return m_pBusData->m_aSeatInfo[i].m_nActionSetIndex;
			}
		}
	}
	return -1;
}

VOID CObject_Bus::DetachCharacterInterface(ObjID_t nObjID)
{
	if(nObjID == INVALID_ID)
		return ;

	INT nCount = GetPassengerCount();
	if(nCount > 0)
	{
		INT i;
		for(i = 0; i < nCount; i++)
		{
			if(m_anPassengerIDs[i] == nObjID)
			{
				if(m_abPassengerAttacheds[i] && m_pRenderInterface != NULL)
				{
					CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
					if(pObj != NULL && pObj->GetRenderInterface() != NULL)
					{
						m_pRenderInterface->Detach_Object(pObj->GetRenderInterface());
					}
				}
				m_anPassengerIDs[i] = m_anPassengerIDs[nCount - 1];
				m_abPassengerAttacheds[i] = m_abPassengerAttacheds[nCount - 1];
				m_anPassengerIDs[nCount - 1] = INVALID_ID;
				m_abPassengerAttacheds[nCount - 1] = FALSE;
				return ;
			}
		}
	}
}

BOOL CObject_Bus::AddPassenger(INT nIndex, ObjID_t nPassengerID)
{
	if(nPassengerID == INVALID_ID || nIndex < 0 || nIndex >= GetMaxPassengerCount())
	{
		return FALSE;
	}

	if(m_anPassengerIDs[nIndex] != INVALID_ID)
	{
		RemovePassenger(m_anPassengerIDs[nIndex]);
	}

	m_anPassengerIDs[nIndex] = nPassengerID;
	m_abPassengerAttacheds[nIndex] = FALSE;
	m_nPassengerCount++;
	UpdateMemberRenderInterfaceByIndex(nIndex);
	return TRUE;
}

BOOL CObject_Bus::RemovePassenger(ObjID_t nPassengerID)
{
	if(nPassengerID == INVALID_ID)
		return FALSE;

	INT nCount = GetPassengerCount();
	if(nCount > 0)
	{
		INT i;
		for(i = 0; i < nCount; i++)
		{
			if(m_anPassengerIDs[i] == nPassengerID)
			{
				if(m_abPassengerAttacheds[i] && m_pRenderInterface != NULL)
				{
					CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
					if(pObj != NULL && pObj->GetRenderInterface() != NULL)
					{
						if(pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
						{
							((CObject_Character*)pObj)->Bus_UpdateCharActionSetFile();
						}
						m_pRenderInterface->Detach_Object(pObj->GetRenderInterface());
					}
				}
				m_anPassengerIDs[i] = m_anPassengerIDs[nCount - 1];
				m_abPassengerAttacheds[i] = m_abPassengerAttacheds[nCount - 1];
				m_anPassengerIDs[nCount - 1] = INVALID_ID;
				m_abPassengerAttacheds[nCount - 1] = FALSE;
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID CObject_Bus::RemoveAllPassengers(VOID)
{
	INT i;
	for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
	{
		if(m_anPassengerIDs[i] != INVALID_ID)
		{
			if(m_abPassengerAttacheds[i] && m_pRenderInterface != NULL)
			{
				CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
				if(pObj != NULL && pObj->GetRenderInterface() != NULL)
				{
					if(pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
					{
						((CObject_Character*)pObj)->Bus_UpdateCharActionSetFile();
					}
					m_pRenderInterface->Detach_Object(pObj->GetRenderInterface());
				}
			}
			m_anPassengerIDs[i] = INVALID_ID;
			m_abPassengerAttacheds[i] = FALSE;
		}
	}
	m_nPassengerCount = 0;
}

VOID CObject_Bus::UpdateMemberRenderInterfaceByIndex(INT nIndex)
{
	if(m_anPassengerIDs[nIndex] == INVALID_ID)
		return;

	if(m_abPassengerAttacheds[nIndex])
		return;

	if(m_pRenderInterface != NULL && m_pBusData != NULL && strlen(m_pBusData->m_aSeatInfo[nIndex].m_pszLocator) > 0)
	{
		CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[nIndex]));
		if(pObj != NULL && pObj->GetRenderInterface() != NULL)
		{
			m_pRenderInterface->Attach_Object(pObj->GetRenderInterface(), m_pBusData->m_aSeatInfo[nIndex].m_pszLocator);
			if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			{
				CObject_Character *pCharMember = (CObject_Character*)(pObj);
				pCharMember->GetCharacterData()->Set_BusObjID(GetServerID());
				pCharMember->Bus_UpdateCharActionSetFile();
			}
			m_abPassengerAttacheds[nIndex] = TRUE;

			UpdateAnimation();
			SetPosition(GetPosition());
		}
	}
}

VOID CObject_Bus::UpdateData(VOID)
{
	if(m_pBusData != NULL && m_pBusData->m_nDataID == m_nDataID)
		return;

	ReleaseRenderInterface();

	m_pBusData = NULL;
	if(m_nDataID != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pBusDataDBC, DBC_BUS_DATA);
		m_pBusData = (const _BUS_INFO*)s_pBusDataDBC->Search_Index_EQU(m_nDataID);
	}

	CreateRenderInterface();
}

VOID CObject_Bus::UpdatePassengerAttached(VOID)
{
	if(m_pRenderInterface != NULL && m_pBusData != NULL)
	{
		INT i;
		for(i = 0; i < m_nPassengerCount; i++)
		{
			if(!m_abPassengerAttacheds[i] && strlen(m_pBusData->m_aSeatInfo[i].m_pszLocator) > 0)
			{
				UpdateMemberRenderInterfaceByIndex(i);
			}
		}
	}
}

VOID CObject_Bus::CreateRenderInterface(VOID)
{
	if(m_pBusData != NULL && m_pBusData->m_nModelID != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		LPCSTR lpszModelFileName = NULL;
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(m_pBusData->m_nModelID));
		if (pCharModel != NULL)
		{
			lpszModelFileName = pCharModel->m_pszModelName;
		}

		if (lpszModelFileName != NULL)
		{
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->SetData(GetID());

			m_pRenderInterface->Actor_SetFile(lpszModelFileName);

			setVisible(isVisible());

			m_pRenderInterface->SetRayQuery(FALSE);

			BOOL bEnable = (isVisible() && CGameProcedure::s_pVariableSystem->GetAs_Int("View_HumanLightmap") == 1);
			if(bEnable)
			{
				m_pRenderInterface->Attach_ProjTexture(tEntityNode::SHADOW_BLOB, GetShadowRange());
			}

			m_pRenderInterface->SetAnimationEndEvent( (FUNC_ONANIMATIONENDEX)(CObject_Bus::_OnAnimationEnd), (DWORD)GetID() ,0);

			UpdatePassengerAttached();

			UpdateCharActionSetFile();

			SetPosition(GetPosition());
			SetFaceDir( GetFaceDir() );
			setVisible(isVisible());
		}
	}
}

VOID CObject_Bus::ReleaseRenderInterface(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		INT i;
		for(i = 0; i < m_nPassengerCount; i++)
		{
			if(m_abPassengerAttacheds[i])
			{
				CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
				if(pObj != NULL && pObj->GetRenderInterface() != NULL)
				{
					if(pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
					{
						((CObject_Character*)pObj)->GetCharacterData()->Set_BusObjID(INVALID_ID);
						((CObject_Character*)pObj)->Bus_UpdateCharActionSetFile();
					}
					m_pRenderInterface->Detach_Object(pObj->GetRenderInterface());
				}
				m_abPassengerAttacheds[i] = FALSE;
			}
		}
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
}

VOID CObject_Bus::UpdateCharActionSetFile(VOID)
{
	m_pCharActionSetFile	= NULL;

	if(m_pBusData != NULL)
	{
		INT nCharModelID	= m_pBusData->m_nModelID;

		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		if(nCharModelID != INVALID_ID)
		{
			const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nCharModelID));
			if (pCharModel != NULL)
			{
				const CHAR *pszActionSetFileName = NULL;
				if(strlen(pCharModel->m_pszActionSetName_None) > 0)
				{
					pszActionSetFileName = pCharModel->m_pszActionSetName_None;
				}

				if(pszActionSetFileName != NULL)
				{
					m_pCharActionSetFile = GetActionSetMgr()->GetActionSetFile(pszActionSetFileName);
				}
			}
		}
	}
}

LPCSTR CObject_Bus::GetCharActionNameByActionSetID(INT nActionSetID)
{
	INT nCalcWeaponType = WEAPON_TYPE_NPC;
	if(m_pCharActionSetFile != NULL && nActionSetID != -1 && nCalcWeaponType >= 0 && nCalcWeaponType < MAX_WEAPON_TYPE_NUMBER)
	{
		const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(m_pCharActionSetFile->Search_Index_EQU(nActionSetID));
		if(pActionSet != NULL)
		{
			return pActionSet->pWeapon_Set[nCalcWeaponType];
		}
	}
	return NULL;
}

VOID CObject_Bus::ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam )
{
	LPCSTR lpszCharActionName = NULL;
	if(m_pRenderInterface != NULL)
	{
		lpszCharActionName = GetCharActionNameByActionSetID(nSetID);
		if(lpszCharActionName == NULL)
			return;
	}

	m_nCurrentAnimationIndex = nSetID;

	if(m_pRenderInterface != NULL)
	{
		m_pRenderInterface->Actor_EnterSkill(TRUE, lpszCharActionName, bLoop, fFuseParam);
	}

	if(m_pRenderInterface)
	{
		m_pRenderInterface->Actor_ChangeActionRate( fSpeed );
	}

	INT i;
	for(i = 0; i < m_nPassengerCount; i++)
	{
		if(m_abPassengerAttacheds[i])
		{
			CObject_Character *pObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_anPassengerIDs[i]));
			if(pObj != NULL && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
			{
				pObj->Bus_ChangeAction(nSetID, fSpeed, bLoop, fFuseParam);

				//pObj->GetRenderInterface()->Actor_RecaculateBoardPos(pObj->GetBoardHeight());
			}
		}
	}
}

VOID CObject_Bus::ChangeActionLoop(BOOL bLoop)
{
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Actor_ChangeActionLoop(bLoop?true:false);
	}
}

bool CObject_Bus::_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam)
{
	//取得该Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Bus)))
		//调用相应的对象函数
		return (((CObject_Bus*)pObj)->OnAnimationEnd(szAnimationName))?(true):(false);
	else
		//该对象已经销毁?
		return false;
}

BOOL CObject_Bus::OnAnimationEnd(LPCTSTR szAnimationName)
{
	UpdateAnimation();
	return true;
}

VOID CObject_Bus::UpdateAnimation(VOID)
{
	if(m_bMoving)
	{
		ChangeAction(BASE_ACTION_N_RUN, 1.f, TRUE);
	}
	else
	{
		INT nActIndex = BASE_ACTION_N_IDLE;
		INT nRandValue = rand()%100;
		if(nRandValue> 90)
		{
			nActIndex = BASE_ACTION_N_IDLE_EX0;
		}
		else if(nRandValue > 80)
		{
			nActIndex = BASE_ACTION_N_IDLE_EX1;
		}

		ChangeAction(nActIndex, 1.f, TRUE);

	}
}
