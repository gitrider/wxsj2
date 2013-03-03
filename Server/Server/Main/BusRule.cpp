// BusRule.cpp
#include "stdafx.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Obj_Bus.h"
#include "Patrol.h"
#include "SceneManager.h"
#include "BusManager.h"

#include "BusRule.h"

ObjID_t BusRule::CreateBusByPatrolPathID(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, INT nPatrolPathID, BOOL bAthwart)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusDataID == INVALID_ID || nPatrolPathID == INVALID_ID)
	{
		return INVALID_ID;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return INVALID_ID;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->NewObject(Obj::OBJ_TYPE_BUS));
	if(pBus == NULL)
	{
		return INVALID_ID;
	}

	_OBJ_BUS_INIT tBusInit;
	// È¡Î»ÖÃ
	{
		PatrolPathMgr *pPatrolPathMgr = pScene->GetPatrolPathMgr();
		if(pPatrolPathMgr == NULL)
		{
			return INVALID_ID;
		}

		const PatrolPath *pPatrolPath = pPatrolPathMgr->GetPatrolPath(nPatrolPathID);
		if(pPatrolPath == NULL)
		{
			return INVALID_ID;
		}

		if(pPatrolPath->GetPatrolPointCount() <= 0)
		{
			return INVALID_ID;
		}

		INT nPathNodeIndex;
		if(bAthwart)
		{
			nPathNodeIndex = pPatrolPath->GetPatrolPointCount() - 1;
		}
		else
		{
			nPathNodeIndex = 0;
		}

		const PatrolPath::PatrolUnit *pPatrolUnit = pPatrolPath->GetUnit(nPathNodeIndex);
		tBusInit.m_Pos		= pPatrolUnit->m_PatrolPoint;
		tBusInit.m_Dir		= 0.f;

		Assert( tBusInit.m_Pos.m_fX >=0.f && tBusInit.m_Pos.m_fZ >= 0.f ) ;
	}

	tBusInit.m_uGUID			= uGUID;
	tBusInit.m_nDataID			= nBusDataID;
	tBusInit.m_bAthwart			= bAthwart;
	tBusInit.m_nPatrolPathID	= nPatrolPathID;

	BOOL bResult = pBus->Init(&tBusInit);
	if(!bResult)
	{
		pBus->CleanUp();
		pScene->DeleteObject(pBus);
		return INVALID_ID;
	}

	pBus->SetActiveFlag(TRUE);
	return pBus->GetID();
__LEAVE_FUNCTION
	return INVALID_ID;
}

ObjID_t BusRule::CreateBus(SceneID_t nSceneID, GUID_t uGUID, INT nBusDataID, const WORLD_POS *pPos, FLOAT fDir)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusDataID == INVALID_ID || pPos == NULL)
	{
		return INVALID_ID;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return INVALID_ID;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->NewObject(Obj::OBJ_TYPE_BUS));
	if(pBus == NULL)
	{
		return INVALID_ID;
	}

	_OBJ_BUS_INIT tBusInit;
	tBusInit.m_Pos				= *pPos;
	tBusInit.m_Dir				= fDir;
	tBusInit.m_uGUID			= uGUID;
	tBusInit.m_nDataID			= nBusDataID;
	tBusInit.m_bAthwart			= FALSE;
	tBusInit.m_nPatrolPathID	= INVALID_ID;

	Assert( pPos->m_fX>=0.f && pPos->m_fZ>=0.f ) ;

	BOOL bResult = pBus->Init(&tBusInit);
	if(!bResult)
	{
		pBus->CleanUp();
		pScene->DeleteObject(pBus);
		return INVALID_ID;
	}

	pBus->SetActiveFlag(TRUE);
	return pBus->GetID();
__LEAVE_FUNCTION
	return INVALID_ID;
}

BOOL BusRule::DeleteBus(SceneID_t nSceneID, ObjID_t nBusObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID)
	{
		return FALSE;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return FALSE;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus != NULL)
	{
		if(pBus->GetObjType() == Obj::OBJ_TYPE_BUS)
		{
			pBus->SetActiveFlag(FALSE);
			pBus->CleanUp();
			pScene->DeleteObject(pBus);
			return TRUE;
		}
	}
	return FALSE;
__LEAVE_FUNCTION
	return FALSE;
}

ObjID_t BusRule::GetBusObjIDByGUID(SceneID_t nSceneID, GUID_t uGUID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID)
	{
		return INVALID_ID;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return INVALID_ID;
	}

	Obj_Bus *pBus = pScene->GetBusManager()->GetBusByGUID(uGUID);
	if(pBus == NULL)
	{
		return INVALID_ID;
	}
	return pBus->GetID();
__LEAVE_FUNCTION
	return INVALID_ID;
}

ORESULT BusRule::TestAddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID || nTargetObjID == INVALID_ID)
	{
		return OR_ERROR;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return OR_ERROR;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return OR_ERROR;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return OR_ERROR;
	}

	Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(nTargetObjID));
	if(pHuman == NULL)
	{
		return OR_ERROR;
	}

	if(pHuman->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		return OR_ERROR;
	}

	if(pHuman->IsInBus())
	{
		return OR_BUS_HASPASSENGER;
	}

	if(pHuman->GetMountID() != INVALID_ID)
	{
		return OR_BUS_HASMOUNT;
	}

	if(pHuman->GetModelID() != INVALID_ID)
	{
		return OR_BUS_CANNOT_CHANGE_MODEL;
	}

	//if(pHuman->IsDRide())
	//{
	//	return OR_BUS_CANNOT_DRIDE;
	//}

	//if(pHuman->GetPet() != NULL)
	//{
	//	return OR_BUS_HASPET;
	//}

	if(pHuman->__GetTeamFollowFlag())
	{
		return OR_BUS_CANNOT_TEAM_FOLLOW;
	}

	if(pBus->IsPassengerFull())
	{
		return OR_BUS_PASSENGERFULL;
	}
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT BusRule::AddPassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID || nTargetObjID == INVALID_ID)
	{
		return OR_ERROR;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return OR_ERROR;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return OR_ERROR;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return OR_ERROR;
	}

	Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(nTargetObjID));
	if(pHuman == NULL)
	{
		return OR_ERROR;
	}

	if(pHuman->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		return OR_ERROR;
	}

	ORESULT oResult = TestAddPassenger(nSceneID, nBusObjID, nTargetObjID);
	if(OR_FAILED(oResult))
	{
		return oResult;
	}

	BOOL bResult = pBus->AddPassenger(nTargetObjID);
	if(!bResult)
	{
		return OR_BUS_HASPASSENGER;
	}

	pHuman->SetBusObjID(pBus->GetID());

	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT BusRule::TestRemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID || nTargetObjID == INVALID_ID)
	{
		return OR_ERROR;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return OR_ERROR;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return OR_ERROR;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return OR_ERROR;
	}

	Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(nTargetObjID));
	if(pHuman == NULL)
	{
		return OR_ERROR;
	}

	if(pHuman->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		return OR_ERROR;
	}

	if(!pHuman->IsInBus())
	{
		return OR_BUS_NOTINBUS;
	}

	if(!pBus->IsPassengerInThis(pHuman->GetID()))
	{
		return OR_BUS_NOTINBUS;
	}
	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

ORESULT BusRule::RemovePassenger(SceneID_t nSceneID, ObjID_t nBusObjID, ObjID_t nTargetObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID || nTargetObjID == INVALID_ID)
	{
		return OR_ERROR;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return OR_ERROR;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return OR_ERROR;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return OR_ERROR;
	}

	Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(nTargetObjID));
	if(pHuman == NULL)
	{
		return OR_ERROR;
	}

	if(pHuman->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		return OR_ERROR;
	}

	ORESULT oResult = TestRemovePassenger(nSceneID, nBusObjID, nTargetObjID);
	if(OR_FAILED(oResult))
	{
		return oResult;
	}

	BOOL bResult = pBus->RemovePassenger(nTargetObjID);
	if(!bResult)
	{
		return OR_BUS_NOTINBUS;
	}

	//pHuman->Teleport(pBus->getWorldPos());
	pHuman->SetBusObjID(INVALID_ID);

	return OR_OK;
__LEAVE_FUNCTION
	return OR_ERROR;
}

VOID BusRule::DebusAllPassenger(SceneID_t nSceneID, ObjID_t nBusObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID)
	{
		return ;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return ;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return ;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return ;
	}

	INT nPassengerCount = pBus->GetPassengerCount();
	INT i;
	for(i = 0; i < nPassengerCount; i++)
	{
		ObjID_t nPassengerID = pBus->GetPassengerIDByIndex(i);
		if(nPassengerID != INVALID_ID)
		{
			Obj_Human *pHuman = (Obj_Human*)(pScene->GetObjManager()->GetObj(nPassengerID));
			if(pHuman != NULL && pHuman->GetObjType() == Obj::OBJ_TYPE_HUMAN)
			{
				//pHuman->Teleport(pBus->getWorldPos());

				pHuman->SetBusObjID(INVALID_ID);
			}
		}
	}

	pBus->RemoveAllPassengers();
__LEAVE_FUNCTION
}

VOID BusRule::StartPatrolPath(SceneID_t nSceneID, ObjID_t nBusObjID)
{
__ENTER_FUNCTION
	if(nSceneID == INVALID_ID || nBusObjID == INVALID_ID)
	{
		return ;
	}

	Scene* pScene = (Scene*)(g_pSceneManager->GetScene(nSceneID));
	if(pScene == NULL)
	{
		return ;
	}

	Obj_Bus *pBus = (Obj_Bus*)(pScene->GetObjManager()->GetObj(nBusObjID));
	if(pBus == NULL)
	{
		return ;
	}

	if(pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		return ;
	}

	pBus->StartPatrolPath();
__LEAVE_FUNCTION
}
