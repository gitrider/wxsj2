/********************************************************************************
 *	文件名：	GCGuild.cpp
 *	全路径：	d:\Prj\Common\Packets\GCGuild.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	12:02
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GCGuild.h"

BOOL GCGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GCGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
