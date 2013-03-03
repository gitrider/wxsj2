/********************************************************************************
 *	文件名：	GCAskTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\GCAskTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	16:10
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include "GCAskTeamFollow.h"


BOOL	GCAskTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCAskTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCAskTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCAskTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

