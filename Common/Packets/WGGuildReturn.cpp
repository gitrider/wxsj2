/********************************************************************************
 *	文件名：	WGGuildReturn.cpp
 *	全路径：	d:\Prj\Common\Packets\WGGuildReturn.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	12:11
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGGuildReturn.h"

BOOL	WGGuildReturn::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Read(iStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildReturn::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildReturn.Write(oStream);

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildReturn::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildReturnHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
