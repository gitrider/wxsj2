/********************************************************************************
 *	文件名：	GCReturnTeamFollow.cpp
 *	全路径：	d:\Prj\Common\Packets\GCReturnTeamFollow.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	18:05
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "stdafx.h"
#include "GCReturnTeamFollow.h"


BOOL	GCReturnTeamFollow::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Return, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_GUID,sizeof(GUID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCReturnTeamFollow::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Return, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_GUID,sizeof(GUID_t) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCReturnTeamFollow::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCReturnTeamFollowHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

