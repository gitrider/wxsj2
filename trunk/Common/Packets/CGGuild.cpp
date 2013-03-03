/********************************************************************************
 *	文件名：	CGGuild.cpp
 *	全路径：	d:\Prj\Common\Packets\CGGuild.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 11 日	13:07
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "CGGuild.h"

BOOL CGGuild::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_GuildPacket.Read( iStream );
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGGuild::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	m_GuildPacket.Write( oStream );
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGGuild::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGGuildHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
