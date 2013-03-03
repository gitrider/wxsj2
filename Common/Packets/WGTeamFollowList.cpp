/********************************************************************************
 *	文件名：	WGTeamFollowList.cpp
 *	全路径：	d:\Prj\Common\Packets\WGTeamFollowList.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	23:41
 *
 *	功能说明：	
 *	修改记录：
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

