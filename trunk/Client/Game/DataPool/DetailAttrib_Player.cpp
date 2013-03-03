// DetailAttrib_Player.cpp

#include "StdAfx.h"
#include "GITimeSystem.h"
#include "..\Procedure\GameProcedure.h"
#include "SkillDataMgr.h"
#include "DetailAttrib_Player.h"
#include "..\Event\GMEventSystem.h"
#include "..\DBC\GMDataBase.h"
#include "GIException.h"
#include "..\Action\GMActionSystem.h"
#include "GIUISystem.h"
#include "..\Sound\GMSoundSystem.h"

#include "network\netmanager.h"
#include "CGAskMissionList.h"
#include "..\Interface\GMGameInterface_Script.h"

#include "..\Object\Manager\ObjectManager.h"
#include "..\Object\Logic\Character\Obj_PlayerMySelf.h"

using namespace Packets;
using namespace SCRIPT_SANDBOX;

// temp code {
// ��ʽģ��
UINT RunFormula( UINT dwFormulaID, UINT dwParam1, UINT dwParam2, UINT dwParam3 )
{
	return (dwParam3 - dwParam2)/((dwParam1)?(dwParam1):(1)) + dwParam2;
}

INT RunFormula( UINT dwFormulaID, UINT dwParam1, INT nParam2, INT nParam3 )
{
	return (nParam3 - nParam2)/((dwParam1)?(dwParam1):(1)) + nParam2;
}

FLOAT RunFormula( UINT dwFormulaID, UINT dwParam1, FLOAT fParam2, FLOAT fParam3 )
{
	return (fParam3 - fParam2)/((dwParam1)?(dwParam1):(1)) + fParam2;
}


CDetailAttrib_Player::CDetailAttrib_Player( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;

	for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
		m_dwScoutMission[i]	= FALSE;
/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

CDetailAttrib_Player::~CDetailAttrib_Player( VOID )
{
}


BOOL CDetailAttrib_Player::Init( VOID )
{
	return TRUE;
}

VOID CDetailAttrib_Player::Term( VOID )
{
	m_dwCanPickMissionItemIDNum				= 0;
	UINT i;
	for ( i = 0; i < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; i++ )
		m_adwCanPickMissionItemList[i]		= UINT_MAX;

/*
	for ( i = 0; i < COOLDOWN_LIST_SIZE; i++ )
	{
		m_anCoolDownTime[i]			= -1;
		m_anCoolDownTotalTime[i]	= -1;
	}
*/
}

VOID CDetailAttrib_Player::Tick( VOID )
{

}

VOID CDetailAttrib_Player::AddMission( const _OWN_MISSION *pMission )
{
	// �������������
	if( m_listMission.m_Count == MAX_CHAR_MISSION_NUM )
		return ;

	// �ж������б����Ƿ��Ѿ�����
	UINT dwIndex = GetMissionIndexByID( pMission->m_idMission );
	if ( dwIndex != UINT_MAX )
		return ;

	dwIndex = UINT_MAX;
	UINT i;
	for( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if( m_listMission.m_aMission[i].m_idMission == INVALID_ID )
		{
			dwIndex = i;
			break;
		}
	}

	if( dwIndex == UINT_MAX )
		return ;

 	m_listMission.m_aMission[dwIndex] = *pMission;
	m_listMission.m_Count++;

	//�������׷���б�û����,��ôֱ����ӽ�ȥ.
	if( GetSCountNum() <= MAX_MISSION_TRACE )
	{
		m_dwScoutMission[dwIndex] = TRUE;
		if( CGameProcedure::s_pUISystem && !CGameProcedure::s_pUISystem->IsWindowShow("MissionScout") )
		{
			CGameProcedure::s_pUISystem->OpenWindow("MissionScout");
		}
	}
	
	// �������ӣ�����������
	CSoundSystemFMod::_PlayUISoundFunc(70);

	//if( CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog") )
	{
		// 20100412 ModifyCodeBegin
		// CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION );  // questlog.lua��Ҫ���ٴ�һ���������ܸ��������б�
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION, "0"); // ����Ϊ0�������ѽ������б�
		// 20100412 ModifyCodeEnd
	}
}

VOID CDetailAttrib_Player::ModifyMission( const _OWN_MISSION *pMission )
{
	UINT dwIndex = GetMissionIndexByID( pMission->m_idMission );
	if ( dwIndex == UINT_MAX )
		return ;

	m_listMission.m_aMission[dwIndex] = *pMission;
	//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
	{
		// 20100412 ModifyCodeBegin
		// CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION );  // questlog.lua��Ҫ���ٴ�һ���������ܸ��������б�
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION, "0"); // ����Ϊ0�������ѽ������б�
		// 20100412 ModifyCodeEnd
	}
}

VOID CDetailAttrib_Player::ModifyMissionData( const INT* pMissionData )
{

	memcpy((void*)m_listMission.m_aMissionData,(void*)pMissionData,sizeof(m_listMission.m_aMissionData));

	//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
	{
		// 20100412 ModifyCodeBegin
		// CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION );  // questlog.lua��Ҫ���ٴ�һ���������ܸ��������б�
		CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION, "0"); // ����Ϊ0�������ѽ������б�
		// 20100412 ModifyCodeEnd
	}
}

extern void RemoveMissionInfo(INT nIndex);

VOID CDetailAttrib_Player::RemoveMission( MissionID_t idMission )
{
	UINT dwIndex = GetMissionIndexByID( idMission );
	if ( dwIndex != UINT_MAX && m_listMission.m_aMission[dwIndex].m_idMission != INVALID_ID )
	{
		RemoveMissionInfo(dwIndex);
		m_listMission.m_aMission[dwIndex].Cleanup();
		m_listMission.m_Count--;

		//ɾ������׷��
		if (m_dwScoutMission[dwIndex])
		{
			m_dwScoutMission[dwIndex] = FALSE;
		}

		//if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("QuestLog"))
		{
			// 20100412 ModifyCodeBegin
			// CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION );  // questlog.lua��Ҫ���ٴ�һ���������ܸ��������б�
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_MISSION, "0"); // ����Ϊ0�������ѽ������б�
			// 20100412 ModifyCodeEnd
		}
	}
}

VOID CDetailAttrib_Player::UpdateCanPickMissionItemList( UINT dwItemCount, const UINT *paItemList )
{
	m_dwCanPickMissionItemIDNum = dwItemCount;
	memcpy( m_adwCanPickMissionItemList, paItemList, sizeof( UINT ) * m_dwCanPickMissionItemIDNum );
}

VOID CDetailAttrib_Player::AddCanPickMissionItem( UINT dwItemDataID )
{
	if ( m_dwCanPickMissionItemIDNum < MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM )
	{
		m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum++] = dwItemDataID;
	}
}

VOID CDetailAttrib_Player::RemoveCanPickMissionItem( UINT dwItemDataID )
{
	UINT i;
	for ( i= 0; i < m_dwCanPickMissionItemIDNum; i++ )
	{
		if ( m_adwCanPickMissionItemList[i] == dwItemDataID )
		{
			m_dwCanPickMissionItemIDNum--;
			m_adwCanPickMissionItemList[i] = m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum];
			m_adwCanPickMissionItemList[m_dwCanPickMissionItemIDNum]	= UINT_MAX;
			return ;
		}
	}
}


UINT CDetailAttrib_Player::GetMissionIndexByID( MissionID_t idMission )const
{
	UINT i;
	for( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
	{
		if ( m_listMission.m_aMission[i].m_idMission == idMission )
		{
			return i;
		}
	}
	return UINT_MAX;
}


VOID CDetailAttrib_Player::OnMissionChanged( UINT dwIndex )
{
}

// �Ƿ��Ѿ�������ĳ������
BOOL CDetailAttrib_Player::IsMissionHaveDone( MissionID_t idMission ) const
{
	// 20100510 ModifyCodeBegin
	MissionID_t nMissionId = idMission;
	MissionID_t idIndex;

	INT		nBitPass;
	INT		nRet;

	nRet = 0;

	idIndex = idMission >> 5;

	Assert( idIndex < MAX_CHAR_MISSION_FLAG_LEN );

	nBitPass = (UINT)nMissionId & 0x0000001F;

	nRet = m_listMission.m_aMissionHaveDoneFlags[idIndex] & (0x00000001 << nBitPass);	

	if ( idIndex < MAX_CHAR_MISSION_FLAG_LEN )
	{
		return ( nRet != 0 );
	}
	else
		return FALSE;

	// 20100510 ModifyCodeEnd
}

// ����������ɲ���
VOID CDetailAttrib_Player::SetMissionHaveDone( const UINT* pHaveDone )
{
	memcpy( m_listMission.m_aMissionHaveDoneFlags, pHaveDone, MAX_CHAR_MISSION_FLAG_LEN );
}

VOID CDetailAttrib_Player::SetMissionComplete( MissionID_t nID, BOOL bComplete )
{
	if( bComplete) 
	{
		m_listMission.m_aMissionHaveDoneFlags[nID >> 5] |= (0x00000001 << ((UINT)nID & 0x0000001F));
	}
	else
	{
		m_listMission.m_aMissionHaveDoneFlags[nID >> 5] &= ~(0x00000001 << ((UINT)nID & 0x0000001F));
	}
}

// 20100510 AddCodeBegin
// �����Ƿ���ɿɽ�
INT CDetailAttrib_Player::IsMissionCanCommit( MissionID_t nID ) const
{
	INT nIndex;
	INT nMissionParaIndex; // ��������ɵı�־λ����Ҫ�ű����������ʱ����

	BOOL bIsDayTipsMission = FALSE;
	BOOL bIsTodayCanSee    = FALSE;

	nMissionParaIndex = 7; // ��Ϊ���������������һ��m_listMission.m_aMission->m_anParam[7]

	nIndex = GetMissionIndexByID( nID );

	if( (nIndex != UINT_MAX) && (GetMissionParam( nIndex, nMissionParaIndex ) == 1) )
		return 1;
	else
		return 0;
}
// 20100510 AddCodeEnd 

// �Ƿ���ÿ�ջ���� 20100728 BLL 
VOID CDetailAttrib_Player::IsDayTipsMission( INT nID, BOOL& bIsDayTipsMission, BOOL& bIsTodayCanSee ) const
{
	BOOL IsToday  = FALSE;
	BOOL IsCanSee = FALSE;

	SCRIPT_SANDBOX::DaytipsData::s_DaytipsData.IsDayTipsMission( nID, IsToday, IsCanSee );

	bIsDayTipsMission = IsToday;
	bIsTodayCanSee    = IsCanSee;
}

// �µ�һ��ͬ������ÿ�ջ����
VOID CDetailAttrib_Player::RefreshDayTipMis() const
{
	time_t  clientTime;
	time(&clientTime);
	tm* tmNow  = localtime(&clientTime);

	// �µ�һ��ͬ������ÿ�ջ����
	if ( tmNow->tm_hour == 0 && tmNow->tm_min == 0 && tmNow->tm_sec == 0 )
	{		
		CGAskMissionList	msgMissionList;
		msgMissionList.setTargetID( CObjectManager::GetMe()->GetMySelf()->GetServerID() );
		CNetManager::GetMe()->SendPacket(&msgMissionList);
	}	
}


// ��ȡ�Ѳμ�һ���Ĵ���    20100728 BLL
INT  CDetailAttrib_Player::GetMissionDoneCount( INT nMissionID ) const
{	
	INT nValue = 0;
	INT nIndex = -1;

	if ( nMissionID <= 0 )
	{
		return nValue;
	}

	_DAY_TIPS_ATTR_TBL* pAttr = DaytipsDataMgr::GetMe()->GetDayTipsTBL();

	for ( INT i = 0; i < pAttr->m_Count; i++ )
	{
		if ( pAttr->m_TableExt[i].m_MisPos == nMissionID )
		{
			nValue = GetMissionData( pAttr->m_TableExt[i].m_DoneCountIndex );

			return nValue;
		}
	}

	return nValue;
}