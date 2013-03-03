/********************************************************************************
 *	文件名：	Team.cpp
 *	全路径：	d:\Prj\Server\Server\Team\Team.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 4 日	9:25
 *
 *	功能说明：	队伍
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "Team.h"
#include "Obj_Human.h"

// 入队
VOID TeamInfo::AddMember( const TEAMMEMBER* pMember )
{
__ENTER_FUNCTION

	if( m_MemberCount>=MAX_TEAM_MEMBER )
	{
		Assert(FALSE);
		return;
	}

	m_aMember[m_MemberCount++] = *pMember;

	if( pMember->m_SceneID == m_MySceneID
		&& pMember->m_GUID != m_MyGUID // 自己不需要加入同场景队友
		)
	{
		AddSceneMember(pMember->m_ObjID);
	}

__LEAVE_FUNCTION
}

// 出队
VOID TeamInfo::DelMember( GUID_t guid )
{
__ENTER_FUNCTION

	BOOL bFind = FALSE;
	ObjID_t oid;

	INT i;

	for( i=0; i<m_MemberCount; ++i )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			bFind = TRUE;
			oid = m_aMember[i].m_ObjID;
			break;
		}
	}

	if( bFind )
	{
		for(;i<m_MemberCount-1; ++i )
		{
			m_aMember[i] = m_aMember[i+1];
		}

		m_MemberCount --;

		DelSceneMember(oid);

		return;
	}

	return;

__LEAVE_FUNCTION
}

// 任命队长
VOID TeamInfo::Appoint( GUID_t guid )
{
__ENTER_FUNCTION

	TEAMMEMBER LM;
	BOOL flag = FALSE;
	INT i;

	for( i=0; i<m_MemberCount; i++ )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			LM = m_aMember[i];
			flag = TRUE;
			break;
		}
	}

	if ( flag )
	{
		for( ; i>0; --i )
		{
			m_aMember[i] = m_aMember[i-1];
		}

		m_aMember[LEADER_ID] = LM;
	}
__LEAVE_FUNCTION
}

// 队员开始切换场景
VOID TeamInfo::StartChangeScene( GUID_t guid )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_MemberCount; ++i )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			if( guid == m_MyGUID )
			{ // 自己换场景的情况
				m_SceneMemberCount = 0;
				m_MySceneID = INVALID_ID;
			}
			else if( m_aMember[i].m_SceneID == m_MySceneID )
			{ // 别人离开场景的情况
				DelSceneMember( m_aMember[i].m_ObjID );
			}

			m_aMember[i].m_SceneID = INVALID_ID;
			break;
		}
	}

	EraseFollowedMemberPointer( guid );

__LEAVE_FUNCTION
}

// 队员进入新场景
VOID TeamInfo::EnterScene( GUID_t guid, SceneID_t SceneID, ObjID_t oid )
{
__ENTER_FUNCTION

	if ( SceneID == INVALID_ID )
	{
		Assert(FALSE);
		return;
	}

	if( guid == m_MyGUID )
	{ // 自己进入
		for( INT i=0; i<m_MemberCount; i++ )
		{
			if ( m_aMember[i].m_GUID == guid )
			{ // 不能 break，因为还要过滤所有同场景玩家
				m_aMember[i].m_SceneID = SceneID;
				m_aMember[i].m_ObjID = oid;
				break;
			}
			//else if ( m_aMember[i].m_SceneID == SceneID )
			//{ // 如果同场景，加入同场景玩家
			//	AddSceneMember( m_aMember[i].m_ObjID );
			//}
		}
	}
	else
	{ // 别人进入
		for( INT i=0; i<m_MemberCount; i++ )
		{
			if( m_aMember[i].m_GUID == guid )
			{
				m_aMember[i].m_SceneID = SceneID;
				m_aMember[i].m_ObjID = oid;

				if( SceneID == m_MySceneID )
				{
					AddSceneMember( oid );
				}

				break;
			}
		}
	}

__LEAVE_FUNCTION
}

// 队友断线
VOID TeamInfo::MemberOffLine( GUID_t guid )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_MemberCount; ++i )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			if( m_aMember[i].m_SceneID == m_MySceneID )
			{
				DelSceneMember( m_aMember[i].m_ObjID );
			}

			m_aMember[i].m_SceneID = INVALID_ID;
			break;
		}
	}

	EraseFollowedMemberPointer( guid );

__LEAVE_FUNCTION
}

// 增加一个跟随队员
VOID TeamInfo::AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember )
{
__ENTER_FUNCTION

	GUID_t guid = FollowedMember.m_GUID;

	for( INT i=0; i<GetFollowedMembersCount(); ++i )
	{
		if ( guid == m_FollowedMembers[i].m_GUID )
		{ // 更新指针
			m_FollowedMembers[i].m_pHuman = FollowedMember.m_pHuman;
			return;
		}
	}

	m_FollowedMembers[m_nFollowedMembersCount].m_GUID = guid;
	m_FollowedMembers[m_nFollowedMembersCount].m_pHuman = FollowedMember.m_pHuman;

	++m_nFollowedMembersCount;

__LEAVE_FUNCTION
}

// 移出某个跟随队员
VOID TeamInfo::DelFollowedMember( GUID_t guid )
{
__ENTER_FUNCTION

	for( INT i=0; i<GetFollowedMembersCount(); ++i )
	{
		if ( guid == m_FollowedMembers[i].m_GUID )
		{
			for( INT j=i; j<GetFollowedMembersCount()-1; ++j )
			{
				m_FollowedMembers[j].m_GUID = m_FollowedMembers[j+1].m_GUID;
				m_FollowedMembers[j].m_pHuman = m_FollowedMembers[j+1].m_pHuman;
			}

			m_FollowedMembers[--m_nFollowedMembersCount].CleanUp();
			return;
		}
	}

__LEAVE_FUNCTION
}

