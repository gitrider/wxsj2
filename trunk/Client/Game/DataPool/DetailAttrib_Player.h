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

	// ͨ������IDȡ�������������������ֵ�� �޸�����ʱ����UINT_MAX
	UINT GetMissionIndexByID( MissionID_t idMission )const;

	// 20100510 ModifyCodeBegin
	// �Ƿ��Ѿ�������ĳ������
	BOOL IsMissionHaveDone( MissionID_t idMission )const;

	// ����������ɲ���
	VOID SetMissionHaveDone( const UINT* pHaveDone );

	// ���õ����������
	VOID SetMissionComplete( MissionID_t nID, BOOL bComplete );

	// 20100510 ModifyCodeEnd

	// 20100510 AddCodeBegin
	// �����Ƿ���ɿɽ�
	INT IsMissionCanCommit( MissionID_t nID )const;
	// 20100510 AddCodeEnd

	// �Ƿ��ǻ����  20100728 BLL 
	VOID IsDayTipsMission( INT nID, BOOL& bIsDayTipsMission, BOOL& bIsTodayCanSee ) const;

	// �µ�һ��ͬ������ÿ�ջ����  20100728 BLL
	VOID RefreshDayTipMis() const;

	// ��ȡ�Ѳμ�һ���Ĵ���    20100728 BLL
	INT  GetMissionDoneCount( INT nMissionID ) const;

	// �����������
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

	// ȡ�������
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

	// ���������Զ�������
	VOID SetMissionData( UINT dwIndexMission, INT nValue )
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM )
		{
			m_listMission.m_aMissionData[dwIndexMission] = nValue;
		}
	}

	// ȡ�����Զ�������
	INT GetMissionData( UINT dwIndexMission ) const
	{
		Assert( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM );

		if( dwIndexMission < MAX_CHAR_MISSION_DATA_NUM )
			return m_listMission.m_aMissionData[dwIndexMission];
		else
			return FALSE;
	}

	//�������----------------------------------------------
	// player�ĵ�ǰ��������
	INT GetMission_Num() const
	{
		return m_listMission.m_Count;
	}
	

	// ������������б�----------------------------------------------
	BOOL GetScoutMission( INT idIndex )const
	{	
		Assert( idIndex >= 0 &&idIndex < MAX_CHAR_MISSION_NUM );

		return m_dwScoutMission[idIndex];
	}

	// ȡ��������б�
	VOID SetScoutMission( INT idIndex )
	{
		Assert( idIndex >= 0 &&idIndex < MAX_CHAR_MISSION_NUM );

		m_dwScoutMission[idIndex] = !m_dwScoutMission[idIndex];
	}

	//��ȡ����׷�ٸ���
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

	// �����
	_MISSION_LIST		m_listMission;

	// ��������б�
	BOOL				m_dwScoutMission[MAX_CHAR_MISSION_NUM];
	// ���Ա����ͻ������ܿ�����������������ƷID�б�
	UINT				m_dwCanPickMissionItemIDNum;
	UINT				m_adwCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];

};
