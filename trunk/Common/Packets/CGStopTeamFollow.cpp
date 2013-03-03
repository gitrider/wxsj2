/********************************************************************************
 *	文件名：	CGStopTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\CGStopTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	22:48
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include "CGStopTeamFollow.h"


BOOL	CGStopTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGStopTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGStopTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGStopTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

