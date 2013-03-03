/********************************************************************************
 *	�ļ�����	Team.cpp
 *	ȫ·����	d:\Prj\Server\Server\Team\Team.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 4 ��	9:25
 *
 *	����˵����	����
 *	�޸ļ�¼��
*********************************************************************************/

#include "stdafx.h"
#include "Team.h"
#include "Obj_Human.h"

// ���
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
		&& pMember->m_GUID != m_MyGUID // �Լ�����Ҫ����ͬ��������
		)
	{
		AddSceneMember(pMember->m_ObjID);
	}

__LEAVE_FUNCTION
}

// ����
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

// �����ӳ�
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

// ��Ա��ʼ�л�����
VOID TeamInfo::StartChangeScene( GUID_t guid )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_MemberCount; ++i )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			if( guid == m_MyGUID )
			{ // �Լ������������
				m_SceneMemberCount = 0;
				m_MySceneID = INVALID_ID;
			}
			else if( m_aMember[i].m_SceneID == m_MySceneID )
			{ // �����뿪���������
				DelSceneMember( m_aMember[i].m_ObjID );
			}

			m_aMember[i].m_SceneID = INVALID_ID;
			break;
		}
	}

	EraseFollowedMemberPointer( guid );

__LEAVE_FUNCTION
}

// ��Ա�����³���
VOID TeamInfo::EnterScene( GUID_t guid, SceneID_t SceneID, ObjID_t oid )
{
__ENTER_FUNCTION

	if ( SceneID == INVALID_ID )
	{
		Assert(FALSE);
		return;
	}

	if( guid == m_MyGUID )
	{ // �Լ�����
		for( INT i=0; i<m_MemberCount; i++ )
		{
			if ( m_aMember[i].m_GUID == guid )
			{ // ���� break����Ϊ��Ҫ��������ͬ�������
				m_aMember[i].m_SceneID = SceneID;
				m_aMember[i].m_ObjID = oid;
				break;
			}
			//else if ( m_aMember[i].m_SceneID == SceneID )
			//{ // ���ͬ����������ͬ�������
			//	AddSceneMember( m_aMember[i].m_ObjID );
			//}
		}
	}
	else
	{ // ���˽���
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

// ���Ѷ���
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

// ����һ�������Ա
VOID TeamInfo::AddFollowedMember( const _FOLLOWEDMEMBER& FollowedMember )
{
__ENTER_FUNCTION

	GUID_t guid = FollowedMember.m_GUID;

	for( INT i=0; i<GetFollowedMembersCount(); ++i )
	{
		if ( guid == m_FollowedMembers[i].m_GUID )
		{ // ����ָ��
			m_FollowedMembers[i].m_pHuman = FollowedMember.m_pHuman;
			return;
		}
	}

	m_FollowedMembers[m_nFollowedMembersCount].m_GUID = guid;
	m_FollowedMembers[m_nFollowedMembersCount].m_pHuman = FollowedMember.m_pHuman;

	++m_nFollowedMembersCount;

__LEAVE_FUNCTION
}

// �Ƴ�ĳ�������Ա
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

