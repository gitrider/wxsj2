/********************************************************************************
 *	文件名：	CGReturnTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\CGReturnTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	17:46
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include "CGReturnTeamFollow.h"


BOOL	CGReturnTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_uRet, sizeof(UCHAR) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	CGReturnTeamFollow::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_uRet, sizeof(UCHAR) );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	CGReturnTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return CGReturnTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

