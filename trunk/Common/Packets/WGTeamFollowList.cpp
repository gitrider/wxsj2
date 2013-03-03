/********************************************************************************
 *	�ļ�����	WGTeamFollowList.cpp
 *	ȫ·����	d:\Prj\Common\Packets\WGTeamFollowList.cpp
 *	�����ˣ�	����
 *	����ʱ�䣺	2005 �� 11 �� 23 ��	23:41
 *
 *	����˵����	
 *	�޸ļ�¼��
*********************************************************************************/
#include "stdafx.h"
#include "WGTeamFollowList.h"


BOOL	WGTeamFollowList::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	iStream.Read( (CHAR*)&m_Count, sizeof(UCHAR) );

	if ( m_Count>0 && m_Count<=MAX_TEAM_MEMBER )
	{
		iStream.Read( (CHAR*)m_GUIDs, m_Count * sizeof(GUID_t) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	WGTeamFollowList::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
	oStream.Write( (CHAR*)&m_Count, sizeof(UCHAR) );

	if ( m_Count>0 && m_Count<=MAX_TEAM_MEMBER )
	{
		oStream.Write( (CHAR*)m_GUIDs, m_Count * sizeof(GUID_t) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGTeamFollowList::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGTeamFollowListHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

