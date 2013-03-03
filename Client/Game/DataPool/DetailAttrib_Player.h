// DetailAttrib_Player.h
// 
//////////////////////////////////////////////////////

#pragma once

#include <map>
#include "DB_Struct.h"
#include "GameDefine2.h"
#include "GameDefine.h"
#include "DetailAttrib_Base.h"
#include "GIException.h"
#include "GMDataPool.h"

#define MAX_MISSION_TRACE		5

class CDetailAttrib_Player : public CDetailAttrib_Base
{
	friend class CUIDataPool;

public:

	CDetailAttrib_Player( VOID );
	virtual ~CDetailAttrib_Player( VOID );

	virtual BOOL Init( VOID );
	virtual VOID Term( VOID );

	virtual VOID Tick( VOID );

	virtual VOID AddMission( const _OWN_MISSION *pMission );

	virtual VOID ModifyMission( const _OWN_MISSION *pMission );

	virtual VOID ModifyMissionData( const INT* pMissionData );

	virtual VOID RemoveMission( MissionID_t idMission );

	virtual VOID UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList );

	virtual VOID AddCanPickMissionItem( UINT dwItemDataID );

	virtual VOID RemoveCanPickMissionItem( UINT dwItemDataID );

	// 通过任务ID取该任务在任务表表的索引值， 无该任务时返回UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// 20100510 ModifyCodeBegin
	// 是否已经做过了某个任务
	BOOL IsMissionHaveDone( MissionID_t idMission )const;

	// 设置任务完成参数
	VOID SetMissionHaveDone( const UINT* pHaveDone );

	// 设置单个任务完成
	VOID SetMissionComplete( MissionID_t nID, BOOL bComplete );

	// 20100510 ModifyCodeEnd

	// 20100510 AddCodeBegin
	// 任务是否完成可交
	INT IsMissionCanCommit( MissionID_t nID )const;
	// 20100510 AddCodeEnd

	// 是否是活动任务  20100728 BLL 
	VOID IsDayTipsMission( INT nID, BOOL& bIsDayTipsMission, BOOL& bIsTodayCanSee ) const;

	// 新的一天同步更新每日活动数据  20100728 BLL
	VOID RefreshDayTipMis() const;

	// 获取已参加一项活动的次数    20100728 BLL
	INT  GetMissionDoneCount( INT nMissionID ) const;

	// 设置任务参数
	VOID SetMissionParam( UINT dwIndexMission, UINT dwIndexParam, INT nValue )
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
		{
			m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam] = nValue;
			
			if(dwIndexParam == MAX_MISSION_PARAM_NUM-1)
				CDataPool::GetMe()->QuestTimeGroup_UpdateList(dwIndexMission,nValue);
		}
	}

	// 取任务参数
	INT GetMissionParam( UINT dwIndexMission, UINT dwIndexParam ) const
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_NUM && dwIndexParam < MAX_MISSION_PARAM_NUM )
			return m_listMission.m_aMission[dwIndexMission].m_anParam[dwIndexParam];
		else
			return FALSE;
	}

	const _OWN_MISSION *GetMission( UINT dwIndex ) const
	{
		if( dwIndex < MAX_CHAR_MISSION_NUM )
			return &m_listMission.m_aMission[dwIndex];
		else
			return NULL;
	}

	// 设置任务自定义数据
	VOID SetMissionData( UINT dwIndexMission, INT nValue )
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM )
		{
			m_listMission.m_aMissionData[dwIndexMission] = nValue;
		}
	}

	// 取任务自定义数据
	INT GetMissionData( UINT dwIndexMission ) const
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM )
			return m_listMission.m_aMissionData[dwIndexMission];
		else
			return FALSE;
	}

	//任务相关----------------------------------------------
	// player的当前任务数量
	INT GetMission_Num() const
	{
		return m_listMission.m_Count;
	}
	

	// 设置任务跟踪列表----------------------------------------------
	BOOL GetScoutMission( INT idIndex )const
	{	
		Assert( idIndex >= 0 &&idIndex < MAX_CHAR_MISSION_NUM );

		return m_dwScoutMission[idIndex];
	}

	// 取任务跟踪列表
	VOID SetScoutMission( INT idIndex )
	{
		Assert( idIndex >= 0 &&idIndex < MAX_CHAR_MISSION_NUM );

		m_dwScoutMission[idIndex] = !m_dwScoutMission[idIndex];
	}

	//获取任务追踪个数
	INT	GetSCountNum(VOID)
	{
		INT cnt = 0;
		for(INT idIndex=0; idIndex<MAX_CHAR_MISSION_NUM; idIndex++)
		{
			if (m_dwScoutMission[idIndex])
			{
				++cnt;
			}
		}
		return cnt;
	}

protected:

	VOID OnMissionChanged( UINT dwIndex );

protected:

	// 任务表
	_MISSION_LIST		m_listMission;

	// 任务跟踪列表
	BOOL				m_dwScoutMission[MAX_CHAR_MISSION_NUM];
	// 可以被本客户端所能看见怪物掉落的任务物品ID列表
	UINT				m_dwCanPickMissionItemIDNum;
	UINT				m_adwCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];

};
