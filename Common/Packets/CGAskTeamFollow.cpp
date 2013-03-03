/********************************************************************************
 *	文件名：	CGAskTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\CGAskTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 15 日	17:57
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include  "CGAskTeamFollow.h"


BOOL	CGAskTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGAskTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGAskTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGAskTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

