/********************************************************************************
 *	文件名：	GCGuildReturn.cpp
 *	全路径：	d:\Prj\Common\Packets\GCGuildReturn.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	17:35
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GCGuildReturn.h"

BOOL	GCGuildReturn::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	m_GuildReturn.Read(iStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GCGuildReturn::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildReturn.Write(oStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCGuildReturn::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildReturnHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
