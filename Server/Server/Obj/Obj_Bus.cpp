// Obj_Bus.cpp
#include "stdafx.h"
#include "BusRule.h"

#include "GCNewBus.h"
#include "GCNewBus_Move.h"
#include "GCBusMove.h"
#include "GCBusStopMove.h"
#include "GCBusAddPassenger.h"
#include "GCBusRemovePassenger.h"
#include "GCBusRemoveAllPassenger.h"

#include "Patrol.h"
#include "Scene.h"
#include "TimeManager.h"
#include "LuaInterface.h"

#include "Obj_Bus.h"

Obj_Bus::Obj_Bus(VOID)
{
__ENTER_FUNCTION
	m_bRunning				= FALSE;
	m_bAthwart				= FALSE;
	m_uRunningSurplusTime	= UINT_MAX;
	m_nPathNodeIndex		= -1;

	m_uGUID					= INVALID_GUID;
	m_nDataID				= INVALID_ID;
	m_nPatrolPathID			= INVALID_ID;
	m_pBusInfo				= NULL;
	m_nPassengerCount		= 0;
	INT i;
	for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
	{
		m_anPassengerIDs[i] = INVALID_ID;
	}

	for(i = 0; i < DEF_BUS_SCRIPT_PARAM_BUF_SIZE; i++)
	{
		m_anScriptParam[i] = -1;
	}

__LEAVE_FUNCTION
}

Obj_Bus::~Obj_Bus(VOID)
{
}

BOOL Obj_Bus::Init(const _OBJ_INIT *pInit)
{
__ENTER_FUNCTION

	if(!Obj_Dynamic::Init(pInit))
		return FALSE;

	const _OBJ_BUS_INIT *pBusInit = (const _OBJ_BUS_INIT*)pInit;
	m_uGUID					= pBusInit->m_uGUID;
	m_nDataID				= pBusInit->m_nDataID;
	m_nPatrolPathID			= pBusInit->m_nPatrolPathID;
	m_bAthwart				= pBusInit->m_bAthwart;

	m_bRunning				= FALSE;
	m_nPathNodeIndex		= -1;
	m_nPassengerCount		= 0;
	INT i;
	for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
	{
		m_anPassengerIDs[i] = INVALID_ID;
	}

	m_pBusInfo				= NULL;
	if(m_nDataID != INVALID_ID)
	{
		m_pBusInfo = g_BusInfoTbl.GetValidItem(m_nDataID);
	}
	if(m_pBusInfo != NULL && m_pBusInfo->m_nStopTime > 0)
	{
		m_uRunningSurplusTime	= (UINT)(m_pBusInfo->m_nStopTime);
	}
	else
	{
		m_uRunningSurplusTime	= UINT_MAX;
	}

	m_ScriptTimer.BeginTimer(1000, g_pTimeManager->CurrentTime());

	for(i = 0; i < DEF_BUS_SCRIPT_PARAM_BUF_SIZE; i++)
	{
		m_anScriptParam[i] = -1;
	}

	Scene *pScene = getScene();
	if(m_pBusInfo != NULL && m_pBusInfo->m_nScriptID != INVALID_ID && pScene != NULL)
	{
		pScene->GetLuaInterface()->ExeScript_DD(m_pBusInfo->m_nScriptID, "OnInit", (INT)pScene->SceneID(), GetID());
	}
	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

VOID Obj_Bus::CleanUp(VOID)
{
__ENTER_FUNCTION

	m_bRunning				= FALSE;
	m_bAthwart				= FALSE;
	m_uRunningSurplusTime	= UINT_MAX;
	m_nPathNodeIndex		= -1;

	m_uGUID					= INVALID_GUID;
	m_nDataID				= INVALID_ID;
	m_nPatrolPathID			= INVALID_ID;
	m_pBusInfo				= NULL;
	RemoveAllPassengers();

	INT i;
	for(i = 0; i < DEF_BUS_SCRIPT_PARAM_BUF_SIZE; i++)
	{
		m_anScriptParam[i] = -1;
	}

	Obj_Dynamic::CleanUp();

__LEAVE_FUNCTION
}

BOOL Obj_Bus::HeartBeat(UINT uTime)
{
__ENTER_FUNCTION
	if(!Obj_Dynamic::HeartBeat(uTime))
	{
		return FALSE;
	}

	if(!IsRunning() && m_uRunningSurplusTime != UINT_MAX)
	{
		if(GetLogicTime() < m_uRunningSurplusTime)
		{
			m_uRunningSurplusTime -= GetLogicTime();
		}
		else
		{
			m_uRunningSurplusTime = 0;
		}
		if(m_uRunningSurplusTime == 0)
		{
			StartPatrolPath();
		}
	}

	BOOL bModifyPassengerPos = FALSE;
	if(IsRunning())
	{
		BOOL bResult = TickPatrolPath(uTime);
		if(!bResult)
		{
			StopPatrolPath(TRUE);
		}

		bModifyPassengerPos = TRUE;
	}

	if(bModifyPassengerPos && getScene() != NULL)
	{
		INT i;
		INT nCount = GetPassengerCount();
		for(i = 0; i < nCount; i++)
		{
			ObjID_t nPassengerID = GetPassengerIDByIndex(i);
			if(nPassengerID != INVALID_ID)
			{
				Obj *pPassenger = getScene()->GetObjManager()->GetObj(nPassengerID);
				if(pPassenger != NULL)
				{
					pPassenger->setWorldPos(getWorldPos());
				}
			}
		}
	}

	UpdateZone( ) ;


	if(m_ScriptTimer.CountingTimer(uTime))
	{
		Scene *pScene = getScene();
		if(m_pBusInfo != NULL && m_pBusInfo->m_nScriptID != INVALID_ID && pScene != NULL)
		{
			pScene->GetLuaInterface()->ExeScript_DDD(m_pBusInfo->m_nScriptID, "OnTimer", (INT)pScene->SceneID(), GetID(), (INT)m_ScriptTimer.GetTermTime());
		}
	}

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

// 未被击活的Obj所执行的逻辑
BOOL Obj_Bus::HeartBeat_OutZone(UINT uTime)
{
__ENTER_FUNCTION
	BOOL bResult = Obj_Dynamic::HeartBeat_OutZone(uTime);
	if(!bResult)
	{
		return bResult;
	}

	// 不可能出现此状态
	return FALSE;

	return TRUE;
__LEAVE_FUNCTION
	return FALSE;
}

Packet *Obj_Bus::CreateNewObjPacket(VOID)
{
__ENTER_FUNCTION

	if (getScene() != NULL)
	{
		if(IsRunning())
		{
			GCNewBus_Move *pPacket = (GCNewBus_Move*)(getScene()->m_pPacket_NewBus_Move);
			pPacket->Init(GetID(), GetDataID(), getWorldPos(), getDir(), &m_posSaveTarget, m_fSaveTargetY, m_nPassengerCount, m_anPassengerIDs);
			return pPacket;
		}
		else
		{
			GCNewBus *pPacket = (GCNewBus*)(getScene()->m_pPacket_NewBus);
			pPacket->Init(GetID(), GetDataID(), getWorldPos(), getDir(), m_nPassengerCount, m_anPassengerIDs);
			return pPacket;
		}
	}
	else
	{
		Assert(getScene() != NULL && "Obj_Bus::CreateNewObjPacket");
		return NULL;
	}

__LEAVE_FUNCTION

	return NULL;
}

VOID Obj_Bus::DestroyNewObjPacket( Packet *pPacket )
{
}

BOOL Obj_Bus::StartPatrolPath(VOID)
{
	if(getScene() == NULL)
	{
		return FALSE;
	}

	PatrolPathMgr *pPatrolPathMgr = getScene()->GetPatrolPathMgr();
	if(pPatrolPathMgr == NULL)
	{
		return FALSE;
	}

	m_pPatrolPath = pPatrolPathMgr->GetPatrolPath(m_nPatrolPathID);
	if(m_pPatrolPath == NULL)
	{
		return FALSE;
	}

	if(m_pPatrolPath->GetPatrolPointCount() <= 0)
	{
		return FALSE;
	}

	if(m_bAthwart)
	{
		m_nPathNodeIndex = m_pPatrolPath->GetPatrolPointCount() - 1;
	}
	else
	{
		m_nPathNodeIndex = 0;
	}

	const PatrolPath::PatrolUnit *pPatrolUnit = m_pPatrolPath->GetUnit(m_nPathNodeIndex);
	m_posSaveTarget		= pPatrolUnit->m_PatrolPoint;
	m_fSaveTargetY		= pPatrolUnit->m_fHeight;
	m_bRunning			= TRUE;

	if(getScene() != NULL && getZoneID() != INVALID_ID)
	{
		GCBusMove msgBusMove;
		msgBusMove.Init(GetID(), getWorldPos(), &m_posSaveTarget, m_fSaveTargetY);
		getScene()->BroadCast(&msgBusMove, getZoneID());
	}
	return TRUE;
}

VOID Obj_Bus::StopPatrolPath(BOOL bAbortive)
{
	if(bAbortive)
	{
		if(getScene() != NULL && getZoneID() != INVALID_ID)
		{
			GCBusStopMove msgBusStopMove;
			msgBusStopMove.Init(GetID(), getWorldPos());
			getScene()->BroadCast(&msgBusStopMove, getZoneID());
		}
	}

	// 卸载乘客
	if(getScene() != NULL)
	{
		BusRule::DebusAllPassenger(getScene()->SceneID(), GetID());
	}

	if(m_pBusInfo != NULL)
	{
		if(m_pBusInfo->m_nType == BUS_TYPE_SHUTTLE)
		{
			m_bAthwart = !m_bAthwart;
			if(m_pBusInfo->m_nStopTime > 0)
			{
				m_uRunningSurplusTime	= (UINT)(m_pBusInfo->m_nStopTime);
			}
			else
			{
				m_uRunningSurplusTime	= UINT_MAX;
			}
		}
		else
		{
			SetActiveFlag(FALSE);
		}
	}

	m_bRunning			= FALSE;
}

BOOL Obj_Bus::TickPatrolPath(UINT uTime)
{
	if(m_pPatrolPath == NULL)
	{
		return FALSE;
	}

	FLOAT fMoveSpeed = GetMoveSpeed();
	if(fMoveSpeed < 0.01f)
	{
		return FALSE;
	}

	UINT uLogicTime = GetLogicTime() ;
	FLOAT fMoveDist = (fMoveSpeed*uLogicTime)/1000.0f ;
	if(fMoveDist <= 0.01f)
	{
		return TRUE;
	}

	WORLD_POS posTarget, posCur;
	posCur = *getWorldPos();
	posTarget = m_posSaveTarget;

	// 当前位置与当前的目标路径点路径的长度
	FLOAT fDistToTarget = MySqrt( &posCur, &posTarget ) ;

	// 如果这一帧可移动的路径长度小于当前位置到当前的目标路径点路径长度
	INT i;
	for(i = 0; i < 100; i++)
	{
		if(fMoveDist > fDistToTarget)
		{
			if ( !NextPathNode(&m_posSaveTarget, &m_fSaveTargetY) )
			{
				// 目标路径是最后一点
				WORLD_POS posMustTo;
				posMustTo.m_fX = posTarget.m_fX;
				posMustTo.m_fZ = posTarget.m_fZ;
				getScene()->GetMap()->VerifyPos( &posMustTo );

				FLOAT fDir = MyAngle( getWorldPos(), &posMustTo );
				setDir( fDir );

				setWorldPos( &posMustTo );

				StopPatrolPath(FALSE);
				break;
			}
			else
			{
				fMoveDist		-= fDistToTarget;
				posCur			= posTarget;
				posTarget		= m_posSaveTarget;
				fDistToTarget	= MySqrt( &posCur, &posTarget ) ;

				// send msg to client...
				if(getScene() != NULL && getZoneID() != INVALID_ID)
				{
					GCBusMove msgBusMove;
					msgBusMove.Init(GetID(), getWorldPos(), &m_posSaveTarget, m_fSaveTargetY);
					getScene()->BroadCast(&msgBusMove, getZoneID());
				}
			}
		}
		else
		{
			WORLD_POS Cur ;
			if(fDistToTarget > 0.f)
			{
				Cur.m_fX = posCur.m_fX + (fMoveDist*(posTarget.m_fX-posCur.m_fX))/fDistToTarget ;
				Cur.m_fZ = posCur.m_fZ + (fMoveDist*(posTarget.m_fZ-posCur.m_fZ))/fDistToTarget ;
			}
			else
			{
				Cur.m_fX = posTarget.m_fX;
				Cur.m_fZ = posTarget.m_fZ;
			}

			FLOAT fDir = MyAngle( &posCur, &posTarget );
			setDir( fDir );

			getScene()->GetMap()->VerifyPos( &Cur );
			setWorldPos( &Cur );
			break;
		}
	}

	return TRUE;
}

BOOL Obj_Bus::NextPathNode(WORLD_POS *pOutPos, FLOAT *pOutHeight)
{
	if(m_pPatrolPath != NULL)
	{
		if(m_bAthwart)
		{
			m_nPathNodeIndex--;
		}
		else
		{
			m_nPathNodeIndex++;
		}
		if(m_nPathNodeIndex >= 0 && m_nPathNodeIndex < m_pPatrolPath->GetPatrolPointCount())
		{
			const PatrolPath::PatrolUnit *pPatrolUnit = m_pPatrolPath->GetUnit(m_nPathNodeIndex);
			if(pOutPos != NULL)
			{
				*pOutPos = pPatrolUnit->m_PatrolPoint;
				*pOutHeight = pPatrolUnit->m_fHeight;
			}
			return TRUE;
		}
	}
	return FALSE;
}

INT Obj_Bus::GetMaxPassengerCount(VOID)const
{
	if(m_pBusInfo != NULL)
	{
		return m_pBusInfo->m_nMaxPassengerCount;
	}
	return 0;
}

FLOAT Obj_Bus::GetMoveSpeed(VOID)const
{
	if(m_pBusInfo != NULL)
	{
		return m_pBusInfo->m_fMoveSpeed;
	}
	return 0.f;
}

BOOL Obj_Bus::IsPassengerInThis(ObjID_t nPassengerID)
{
	if(nPassengerID == INVALID_ID)
	{
		return FALSE;
	}
	INT i;
	INT nCount = GetPassengerCount();
	for(i = 0; i < nCount; i++)
	{
		if(m_anPassengerIDs[i] == nPassengerID)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL Obj_Bus::AddPassenger(ObjID_t nPassengerID)
{
	if(nPassengerID == INVALID_ID)
	{
		return FALSE;
	}

	if(m_nPassengerCount >= GetMaxPassengerCount())
	{
		return FALSE;
	}

	m_anPassengerIDs[m_nPassengerCount++] = nPassengerID;

	if(getScene() != NULL && getZoneID() != INVALID_ID)
	{
		GCBusAddPassenger msgAddPassenger;
		msgAddPassenger.Init(GetID(), m_nPassengerCount - 1, nPassengerID);
		getScene()->BroadCast(&msgAddPassenger, getZoneID());
	}
	return TRUE;
}

BOOL Obj_Bus::RemovePassenger(ObjID_t nPassengerID)
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
				m_anPassengerIDs[i] = m_anPassengerIDs[nCount - 1];
				m_anPassengerIDs[nCount - 1] = INVALID_ID;

				if(getScene() != NULL && getZoneID() != INVALID_ID)
				{
					GCBusRemovePassenger msgRemovePassenger;
					msgRemovePassenger.Init(GetID(), nPassengerID);
					getScene()->BroadCast(&msgRemovePassenger, getZoneID());
				}
				return TRUE;
			}
		}
	}
	return FALSE;
}

VOID Obj_Bus::RemoveAllPassengers(VOID)
{
	INT i;
	for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
	{
		m_anPassengerIDs[i] = INVALID_ID;
	}
	m_nPassengerCount = 0;

	if(getScene() != NULL && getZoneID() != INVALID_ID)
	{
		GCBusRemoveAllPassenger msgRemoveAllPassenger;
		msgRemoveAllPassenger.Init(GetID());
		getScene()->BroadCast(&msgRemoveAllPassenger, getZoneID());
	}
}

BOOL Obj_Bus::IsChauffeur(ObjID_t nObjID)const
{
	return FALSE;
}

BOOL Obj_Bus::IsPassenger(ObjID_t nObjID)const
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
