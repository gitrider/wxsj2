/********************************************************************************
 *	文件名：	GWGuild.cpp
 *	全路径：	d:\Prj\Common\Packets\GWGuild.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 11 日	20:45
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWGuild.h"

BOOL GWGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
