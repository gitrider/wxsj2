/********************************************************************************
 *	文件名：	WGGuildError.cpp
 *	全路径：	d:\Prj\Common\Packets\WGGuildError.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	9:22
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGGuildError.h"

BOOL	WGGuildError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WGGuildError::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	WGGuildError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return WGGuildErrorHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
