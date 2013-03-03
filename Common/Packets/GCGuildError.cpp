/********************************************************************************
 *	文件名：	GCGuildError.cpp
 *	全路径：	d:\Prj\Common\Packets\GCGuildError.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 12 日	17:37
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GCGuildError.h"

BOOL	GCGuildError::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	GCGuildError::Write(SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_Error), sizeof(m_Error) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCGuildError::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCGuildErrorHandler::Execute(this, pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}
