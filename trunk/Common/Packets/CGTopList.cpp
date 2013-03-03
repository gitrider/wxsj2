/********************************************************************************
 *	文件名：	CGTopList.cpp

 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 10 日	
 *
 *	功能说明：	客户端往服务器端发送的请求排行榜消息
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "CGTopList.h"

BOOL CGTopList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGTopList::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGTopList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGTopListHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
