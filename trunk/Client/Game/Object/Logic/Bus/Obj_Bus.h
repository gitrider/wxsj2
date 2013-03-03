// Obj_Bus.h
// 
/////////////////////////////////////////////////////////

#ifndef __OBJ_BUS_H__
#define __OBJ_BUS_H__

#pragma once

#include "Type.h"
#include "GameStruct_Bus.h"
#include "..\Obj_Dynamic.h"
#include "..\..\ObjectDef.h"

namespace DBC
{
	class DBCFile;
}


struct SObject_BusInit :
	public SObjectInit
{
	INT			m_nDataID;
	INT			m_nPassengerCount;		// 乘客数目
	ObjID_t		m_anPassengerIDs[DEF_BUS_MAX_PASSENGER_COUNT];	// 乘客列表

	SObject_BusInit(VOID)
	{
		m_nDataID			= INVALID_ID;
		m_nPassengerCount	= 0;
		INT i;
		for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
		{
			m_anPassengerIDs[i] = INVALID_ID;
		}
	}

	virtual VOID Reset(VOID)
	{
		m_nDataID			= INVALID_ID;
		m_nPassengerCount	= 0;
		INT i;
		for(i = 0; i < DEF_BUS_MAX_PASSENGER_COUNT; i++)
		{
			m_anPassengerIDs[i] = INVALID_ID;
		}

		SObjectInit::Reset();
	}
};

class CObject_Bus :
	public CObject_Dynamic
{
public:
	CObject_Bus();
	virtual ~CObject_Bus();

	virtual VOID Initial(VOID* pParam);
	virtual VOID Release(VOID);
	virtual VOID Tick(VOID);

	// 阴影大小
	virtual FLOAT GetShadowRange(VOID)  { return 100.0f; }

	virtual VOID SetPosition(const fVector3& fvPosition);
	virtual VOID SetRotation(const fVector3& fvRotation);
public:
	// 压入一条指令
	virtual BOOL PushCommand(const SCommand_Object *pCmd);

protected:
	BOOL StartMove(const fVector3 *pTargetPos);
	VOID StopMove(VOID);

	BOOL Tick_Move(UINT uElapseTime);

protected:
	fVector3		m_fvMoveTargetPos;
	fVector3		m_fvSaveStartPos;
	FLOAT			m_fTargetDir;
	BOOL			m_bMoving;

public:
	INT GetMaxPassengerCount(VOID)const;
	FLOAT GetMoveSpeed(VOID)const;

	INT GetPassengerCount(VOID)const
	{
		return m_nPassengerCount;
	}

	BOOL IsChauffeur(ObjID_t nObjID)const;
	BOOL IsPassenger(ObjID_t nObjID)const;

	INT GetDataID(VOID)const
	{
		return m_nDataID;
	}

	const _BUS_INFO *GetBusData(VOID)const
	{
		return m_pBusData;
	}

	INT GetPassengerActionSetFileIndex(ObjID_t nObjID)const;

	VOID DetachCharacterInterface(ObjID_t nObjID);

protected:
	BOOL AddPassenger(INT nIndex, ObjID_t nPassengerID);
	BOOL RemovePassenger(ObjID_t nPassengerID);
	VOID RemoveAllPassengers(VOID);

	VOID UpdateMemberRenderInterfaceByIndex(INT nIndex);

protected:
	INT			m_nDataID;
	INT			m_nPassengerCount;										// 乘客数目
	ObjID_t		m_anPassengerIDs[DEF_BUS_MAX_PASSENGER_COUNT];			// 乘客列表
	BOOL		m_abPassengerAttacheds[DEF_BUS_MAX_PASSENGER_COUNT];	// 乘客是否已经绑定到了车上

public:
	VOID ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam = 0.3);
	VOID ChangeActionLoop(BOOL bLoop);
	virtual LPCSTR GetCharActionNameByActionSetID(INT nActionSetID);

protected:
	static bool _OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam);
	virtual BOOL OnAnimationEnd(LPCTSTR szAnimationName);

	VOID UpdateAnimation(VOID);

protected:
	INT m_nCurrentAnimationIndex;

protected:
	VOID UpdateData(VOID);
	VOID UpdatePassengerAttached(VOID);
	VOID CreateRenderInterface(VOID);
	VOID ReleaseRenderInterface(VOID);
	VOID UpdateCharActionSetFile(VOID);

protected:
	const DBC::DBCFile	*m_pCharActionSetFile;
	const _BUS_INFO		*m_pBusData;

protected:
	//--------------------------------------------------------
	 KL_DECLARE_DYNAMIC(CObject_Bus);
	 KL_DECLARE_LOGICAL(true);
};

#endif // __OBJ_BUS_H__
