/********************************************************************************
 *	文件名：	GWEnterTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\GWEnterTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	15:23
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include  "GWEnterTeamFollow.h"


BOOL	GWEnterTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GWEnterTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GWEnterTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GWEnterTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

