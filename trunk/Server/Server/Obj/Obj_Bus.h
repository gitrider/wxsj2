// Obj_Bus.h
//
////////////////////////////////////////////////////////

#ifndef __OBJ_BUS_H__
#define __OBJ_BUS_H__

#include "Obj_Dynamic.h"
#include "GameStruct_Bus.h"

struct _OBJ_BUS_INIT :
	public _OBJ_DYNAMIC_INIT
{
	GUID_t		m_uGUID;			// GUID
	INT			m_nDataID;			// 数据ID
	INT			m_nPatrolPathID;	// 路线ID
	BOOL		m_bAthwart;			// 是否为逆向的

	_OBJ_BUS_INIT(VOID)
	{
		m_uGUID			= INVALID_GUID;
		m_nDataID		= INVALID_ID;
		m_nPatrolPathID	= INVALID_ID;
		m_bAthwart		= FALSE;
	}

	virtual VOID CleanUp(VOID)
	{
		m_uGUID			= INVALID_GUID;
		m_nDataID		= INVALID_ID;
		m_nPatrolPathID	= INVALID_ID;
		m_bAthwart		= FALSE;
		_OBJ_DYNAMIC_INIT::CleanUp();
	}
};

struct PatrolPath;
class Obj_Bus :
	public Obj_Dynamic
{
public:
	Obj_Bus(VOID);
	virtual ~Obj_Bus(VOID);

	virtual VOID	CleanUp(VOID);
	virtual BOOL	Init(const _OBJ_INIT *pInit);

	virtual ObjType	GetObjType(VOID)const{ return OBJ_TYPE_BUS; }

	virtual BOOL	HeartBeat(UINT uTime = 0);
	virtual BOOL	HeartBeat_OutZone(UINT uTime=0);

	virtual GUID_t	GetGUID(VOID)const
	{
		return m_uGUID;
	}

public:
	virtual Packet	*CreateNewObjPacket(VOID);
	virtual VOID	DestroyNewObjPacket(Packet *pPacket);

public:
	BOOL IsRunning(VOID)const
	{
		return m_bRunning;
	}

	UINT GetRunningSurplusTime(VOID)const
	{
		return m_uRunningSurplusTime;
	}

	BOOL			StartPatrolPath(VOID);
	VOID			StopPatrolPath(BOOL bAbortive);

protected:
	BOOL			TickPatrolPath(UINT uTime);
	BOOL			NextPathNode(WORLD_POS *pOutPos, FLOAT *pOutHeight);

protected:
	BOOL			m_bRunning;				// 是否已经开动
	BOOL			m_bAthwart;				// 是否为逆向的路线
	UINT			m_uRunningSurplusTime;	// 等待开动的时间
	INT				m_nPathNodeIndex;		// 目标点路径索引
	WORLD_POS		m_posSaveTarget;		// 当前目标点
	FLOAT			m_fSaveTargetY;			// 当前目标点Y

public:
	INT GetMaxPassengerCount(VOID)const;
	FLOAT GetMoveSpeed(VOID)const;

	BOOL IsPassengerFull(VOID)const
	{
		return m_nPassengerCount == GetMaxPassengerCount();
	}

	BOOL IsPassengerInThis(ObjID_t nPassengerID);

	BOOL AddPassenger(ObjID_t nPassengerID);
	BOOL RemovePassenger(ObjID_t nPassengerID);
	VOID RemoveAllPassengers(VOID);

	INT GetPassengerCount(VOID)const
	{
		return m_nPassengerCount;
	}

	ObjID_t GetPassengerIDByIndex(INT nIndex)const
	{
		if(nIndex >= 0 && nIndex < m_nPassengerCount)
		{
			return m_anPassengerIDs[nIndex];
		}
		return INVALID_ID;
	}

	INT GetDataID(VOID)const
	{
		return m_nDataID;
	}

	BOOL IsChauffeur(ObjID_t nObjID)const;
	BOOL IsPassenger(ObjID_t nObjID)const;

public:
	INT GetScriptParam(INT nIndex)const
	{
		if(nIndex >= 0 && nIndex < DEF_BUS_SCRIPT_PARAM_BUF_SIZE)
		{
			return m_anScriptParam[nIndex];
		}
		return -1;
	}

	VOID SetScriptParam(INT nIndex, INT nValue)
	{
		if(nIndex >= 0 && nIndex < DEF_BUS_SCRIPT_PARAM_BUF_SIZE)
		{
			m_anScriptParam[nIndex] = nValue;
		}
	}

protected:
	GUID_t				m_uGUID;				// GUID
	INT					m_nDataID;				// 数据ID
	INT					m_nPatrolPathID;		// 路线ID
	const _BUS_INFO		*m_pBusInfo;			// BUS的信息
	const PatrolPath	*m_pPatrolPath;			// 路线

	INT					m_nPassengerCount;		// 乘客数目
	ObjID_t				m_anPassengerIDs[DEF_BUS_MAX_PASSENGER_COUNT];	// 乘客列表

	CMyTimer			m_ScriptTimer;
	INT					m_anScriptParam[DEF_BUS_SCRIPT_PARAM_BUF_SIZE];
};

#endif	// __OBJ_BUS_H__
