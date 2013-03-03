
//-----------------------------------------------------------------------------
// 文件名 : TeamRecruitManager.cpp
// 模块	:	队伍征兵管理器
// 功能	 :
// 修改历史:
//创建:		王勇鸷
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "TeamRecruitManager.h"

TeamRecruitManager* g_pTeamRecruitManager = 0;

TeamRecruitManager::TeamRecruitManager()
{
	__ENTER_FUNCTION

		CleanUp( );

	__LEAVE_FUNCTION
}


TeamRecruitManager::~TeamRecruitManager()
{
	__ENTER_FUNCTION

		CleanUp( );

	__LEAVE_FUNCTION
}


BOOL TeamRecruitManager::Init( )
{
	__ENTER_FUNCTION

	m_LeaderIDTable.InitTable(MAX_TEAMS);
	m_MemberIDTable.InitTable(MAX_TEAMS);

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

VOID TeamRecruitManager::CleanUp( )
{
	__ENTER_FUNCTION

	m_LeaderIDTable.CleanUp();
	m_MemberIDTable.CleanUp();

	for (INT i=0;i<MAX_TEAMS;i++)
	{
		m_LeaderData[i].CleanUp();
		m_MemberData[i].CleanUp();
	}

	__LEAVE_FUNCTION
}


IDTable* TeamRecruitManager::GetLeaderTable(  )
{
	__ENTER_FUNCTION

		return &m_LeaderIDTable;

	__LEAVE_FUNCTION

		return 0;
}

IDTable* TeamRecruitManager::GetMemberTable(  )
{
	__ENTER_FUNCTION

		return &m_MemberIDTable;

	__LEAVE_FUNCTION

		return 0;
}


//读取信息
VOID* TeamRecruitManager::Get( uint id,UCHAR type ) 
{
	__ENTER_FUNCTION

		if ( type == LEADER_RECRUIT )
		{
			return m_LeaderIDTable.Get(id);
		}
		else 	if ( type == MEMBER_RECRUIT )
		{
			return m_MemberIDTable.Get(id);
		}
		else
		{
			Assert(FALSE);
		}

		__LEAVE_FUNCTION

			return NULL;

}


LEADER_INFO_NODE	TeamRecruitManager::GetLeaderRecruitInfo(INT index)
{
	AssertEx(index>=0&&index<MAX_TEAMS,"GetLeaderRecruitInfo");
	return m_LeaderData[index];
}

MEMBER_INFO_NODE	TeamRecruitManager::GetMemberRecruitInfo(INT index)
{
	AssertEx(index>=0&&index<MAX_TEAMS,"GetMemberRecruitInfo");
	return m_MemberData[index];
}


LEADER_INFO_NODE* TeamRecruitManager::GetLeaderNullNode()
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_TEAMS; i++ )
	{
		if( m_LeaderData[i].m_NodeState==NODE_NULL )
		{
			return &(m_LeaderData[i]);
		}
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}

MEMBER_INFO_NODE* TeamRecruitManager::GetMemberNullNode()
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_TEAMS; i++ )
	{
		if( m_MemberData[i].m_NodeState==NODE_NULL )
		{
			return &(m_MemberData[i]);
		}
	}

	return NULL;

__LEAVE_FUNCTION

	return NULL;
}


BOOL TeamRecruitManager::Add( uint id, VOID* pPtr,UCHAR type ) 
{
	__ENTER_FUNCTION

		BOOL bRet = FALSE;

	if ( type == LEADER_RECRUIT )
	{
		{
			if ( m_LeaderIDTable.Add(id,pPtr) )
			{
				LEADER_INFO_NODE *pData = (LEADER_INFO_NODE*)pPtr;
				pData->m_NodeState = NODE_USING;
				return TRUE;
			}		
		}
	}
	else 	if ( type == MEMBER_RECRUIT )
	{	
		if ( m_MemberIDTable.Add( id,pPtr ) )
		{
			MEMBER_INFO_NODE *pData = (MEMBER_INFO_NODE*)pPtr;
			pData->m_NodeState = NODE_USING;
			return TRUE;		
		}		
	}
	else
	{
		Assert(FALSE);
	}

	return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}


//删除表项
VOID TeamRecruitManager::Remove( uint id,UCHAR type ) 
{
	__ENTER_FUNCTION

		if ( type == LEADER_RECRUIT )
		{
			LEADER_INFO_NODE *pData = (LEADER_INFO_NODE*)m_LeaderIDTable.Get(id);
			if ( pData )
			{
				pData->CleanUp();
				m_LeaderIDTable.Remove(id);
			}
		}
		else 	if ( type == MEMBER_RECRUIT )
		{
			MEMBER_INFO_NODE *pData = (MEMBER_INFO_NODE*)m_MemberIDTable.Get(id);
			if ( pData )
			{
				pData->CleanUp();
				m_MemberIDTable.Remove(id);	
			}
		}
		else
		{
			Assert(FALSE);
		}

		__LEAVE_FUNCTION

}






