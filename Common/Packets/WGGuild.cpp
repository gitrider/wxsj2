/********************************************************************************
 *	文件名：	WGGuild.cpp
 *	全路径：	d:\Prj\Common\Packets\WGGuild.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	11:58
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGGuild.h"

BOOL WGGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL WGGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT WGGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
