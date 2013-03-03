// BusManager.cpp
#include "stdafx.h"
#include "Log.h"
#include "Ini.h"
#include "Scene.h"
#include "ObjManager.h"
#include "Obj_Bus.h"
#include "BusPool.h"
#include "SceneManager.h"
#include "BusRule.h"
#include "Patrol.h"

#include "BusManager.h"

#define SECSIZE 64

BusManager::BusManager(VOID)
{
__ENTER_FUNCTION

	memset(m_pNeedRemoveBuss, 0, sizeof(m_pNeedRemoveBuss));
	m_nRemoveCount = 0;

__LEAVE_FUNCTION
}

BusManager::~BusManager(VOID)
{
__ENTER_FUNCTION

__LEAVE_FUNCTION
}

Obj_Bus *BusManager::GetBus(ObjID_t idObj)
{
__ENTER_FUNCTION

	Obj_Bus *pBus = (Obj_Bus*)(GetScene()->GetObjManager()->GetObj(idObj));
	if (pBus == NULL)
	{
		return NULL;
	}

	if (pBus->GetObjType() != Obj::OBJ_TYPE_BUS)
	{
		Assert((pBus->GetObjType() == Obj::OBJ_TYPE_BUS) && "BusManager::GetBus");
		return NULL;
	}
	return pBus;
__LEAVE_FUNCTION
	return NULL;
}

Obj_Bus *BusManager::GetBusByGUID(GUID_t uGUID)
{
__ENTER_FUNCTION

	Obj_Bus *pBus;
	UINT i, uCount;

	uCount		= GetCount();

	for(i = 0; i < uCount; i++)
	{
		pBus = (Obj_Bus*)(GetObjByIndex(i));
		if(pBus != NULL && pBus->GetGUID() == uGUID)
		{
			return pBus;
		}
	}
	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

BOOL BusManager::HeartBeat(UINT uTime)
{
__ENTER_FUNCTION

	if(!m_LogicTimer.CountingTimer(uTime))
		return TRUE;

	memset(m_pNeedRemoveBuss, 0, sizeof(m_pNeedRemoveBuss));

	m_nRemoveCount = 0;

	INT i;
	for (i = 0; i < (INT)m_uCount; i++)
	{
		if (m_papObjs[i] != NULL)
		{
			if (m_papObjs[i]->IsActiveObj())
			{
				 m_papObjs[i]->HeartBeat(uTime);
			}
			else
			{
				BOOL bRet = m_papObjs[i]->HeartBeat_OutZone(uTime);
				if(bRet==FALSE)
				{
					if(m_nRemoveCount < MAX_REMOVE_SIZE)
					{
						m_pNeedRemoveBuss[m_nRemoveCount++] = (Obj_Bus*)(m_papObjs[i]);
					}
				}
			}
		}
	}

	for(i=m_nRemoveCount-1; i>=0; i--)
	{
		Obj_Bus* pBus = m_pNeedRemoveBuss[i];
		if(pBus != NULL)
		{
			pBus->SetActiveFlag(FALSE);
			pBus->CleanUp();
			GetScene()->DeleteObject(pBus);
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL BusManager::LoadBus(const CHAR *pszFileName)
{
__ENTER_FUNCTION

	Scene	*pScene;
	CHAR	szSection[SECSIZE];
	pScene	= GetScene();
	CHAR	szLog[1024];
	BUS_FILE* pBusFile = g_pSceneManager->FindBusFile(pszFileName);
	if(pBusFile == NULL)
	{
		//找出一个空的
		pBusFile = g_pSceneManager->FindEmptyBusFile();
		if(pBusFile == NULL)
		{
			AssertEx(FALSE, "[BusManager::LoadBus]:Read g_pSceneManager->FindEmptyBusFile() failed!!");
			return FALSE;
		}

		Ini f(pszFileName);
		//读取BusObj数据
		INT iBusCount = 0;
		if(FALSE==f.ReadIntIfExist("info", "buscount", iBusCount))
		{
			iBusCount = 0;
			AssertEx(FALSE, "[BusManager::LoadBus]:Read info::buscount in ini file failed!!");
			return FALSE;
		}
		pBusFile->m_nCount = iBusCount;
		pBusFile->m_paInit = new _OBJ_BUS_INIT[iBusCount];
		if(pBusFile->m_paInit==NULL)
		{
			AssertEx(FALSE, "[BusManager::LoadBus]:alloc memory for _OBJ_BUS_INIT failed!!");
			return FALSE;
		}

		for(INT i=0; i<iBusCount; i++)
		{
			_OBJ_BUS_INIT init;

			sprintf(szSection, "bus%d", i);
			
			INT nReadGUID = -1;
			if(FALSE==f.ReadIntIfExist(szSection, "guid", nReadGUID))
			{
				init.m_uGUID = INVALID_GUID;
				tsnprintf(szLog, sizeof(szLog),"[BusManager::LoadBus]:Read %s::guid failed!!", szSection);
				AssertEx(FALSE, szLog);
				return FALSE;
			}
			init.m_uGUID = (GUID_t)nReadGUID;

			if(FALSE==f.ReadIntIfExist(szSection, "dataId", init.m_nDataID))
			{
				init.m_nDataID = INVALID_ID;
				tsnprintf(szLog, sizeof(szLog),"[BusManager::LoadBus]:Read %s::dataId failed!!", szSection);
				AssertEx(FALSE, szLog);
				return FALSE;
			}

			if(FALSE==f.ReadIntIfExist(szSection, "patrolPathID", init.m_nPatrolPathID))
			{
				init.m_nPatrolPathID = INVALID_ID;
				tsnprintf(szLog, sizeof(szLog),"[BusManager::LoadBus]:Read %s::patrolPathID failed!!", szSection);
				AssertEx(FALSE, szLog);
				return FALSE;
			}

			if(FALSE==f.ReadIntIfExist(szSection, "athwart", init.m_bAthwart))
			{
				init.m_bAthwart = INVALID_ID;
				tsnprintf(szLog, sizeof(szLog),"[BusManager::LoadBus]:Read %s::athwart failed!!", szSection);
				AssertEx(FALSE, szLog);
				return FALSE;
			}

			pBusFile->m_paInit[i] = init;
		}

		strncpy(pBusFile->m_szFileName, pszFileName, _MAX_PATH-1);
		pBusFile->m_szFileName[_MAX_PATH-1] = '\0';
	}

	PatrolPathMgr *pPatrolPathMgr = pScene->GetPatrolPathMgr();
	if(pPatrolPathMgr == NULL)
	{
		return FALSE;
	}

	//生成BUS
	for(INT i = 0; i < pBusFile->m_nCount; i++)
	{
		//BusRule::CreateBusByPatrolPathID(GetScene()->SceneID(), pBusFile->m_paInit[i].m_uGUID, pBusFile->m_paInit[i].m_nDataID, pBusFile->m_paInit[i].m_nPatrolPathID, pBusFile->m_paInit[i].m_bAthwart);

		Obj_Bus *pBus = (Obj_Bus*)(pScene->NewObject(Obj::OBJ_TYPE_BUS));
		if(pBus == NULL)
		{
			return FALSE;
		}

		_OBJ_BUS_INIT tBusInit;
		// 取位置
		{
			const PatrolPath *pPatrolPath = pPatrolPathMgr->GetPatrolPath(pBusFile->m_paInit[i].m_nPatrolPathID);
			if(pPatrolPath == NULL)
			{
				char szOutStr[1024] = {0};
				sprintf(szOutStr, "can not find patrol path in patrol path manager, filename=<%s> partrolId=%d", pszFileName, pBusFile->m_paInit[i].m_nPatrolPathID);
				AssertEx(FALSE, szOutStr);
				continue;
			}

			if(pPatrolPath->GetPatrolPointCount() <= 0)
			{
				char szOutStr[1024] = {0};
				sprintf(szOutStr, "invalid PartolPointCount, filename=<%s> partrolId=%d count =%d", pszFileName, pBusFile->m_paInit[i].m_nPatrolPathID, pPatrolPath->GetPatrolPointCount());
				AssertEx(FALSE, szOutStr);
				continue;
			}

			INT nPathNodeIndex;
			if(pBusFile->m_paInit[i].m_bAthwart)
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
		}

		Assert(tBusInit.m_Pos.m_fX>=0.f && tBusInit.m_Pos.m_fZ>=0.f);

		tBusInit.m_uGUID			= pBusFile->m_paInit[i].m_uGUID;
		tBusInit.m_nDataID			= pBusFile->m_paInit[i].m_nDataID;
		tBusInit.m_bAthwart			= pBusFile->m_paInit[i].m_bAthwart;
		tBusInit.m_nPatrolPathID	= pBusFile->m_paInit[i].m_nPatrolPathID;

		BOOL bResult = pBus->Init(&tBusInit);
		if(!bResult)
		{
			pBus->CleanUp();
			pScene->DeleteObject(pBus);

			char szOutStr[1024] = {0};
			sprintf(szOutStr, "init bus failed, filename=<%s> busTemplateId=%d", pszFileName, pBusFile->m_paInit[i].m_nDataID);
			AssertEx(FALSE, szOutStr);
			continue;
		}

		pBus->SetActiveFlag(TRUE);
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL BusManager::RemoveAllBus()
{
__ENTER_FUNCTION

	Scene *pScene;
	Obj_Bus *pBus;
	UINT i,uCount,uMaxCount,uLength;

	pScene		= GetScene();
	uCount		= 0;
	uMaxCount	= GetCount();
	uLength	= GetLength();

	for (i = 0; i <uLength; i++)
	{
		pBus = (Obj_Bus*)(GetObjByIndex(0));
		if (pBus != NULL)
		{
			pBus->SetActiveFlag(FALSE);
			pBus->CleanUp();
			pScene->DeleteObject(pBus);
			uCount++;
			if (uCount >=uMaxCount)
				break;
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}
