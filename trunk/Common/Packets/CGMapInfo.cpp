/********************************************************************************
 *	文件名：	CGMapInfo.cpp

 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 16 日	
 *
 *	功能说明：	客户端往服务器端发送的请求地图(用于跳转)消息
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "CGMapInfo.h"

BOOL CGMapInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Index, sizeof(m_Index) );	

	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL CGMapInfo::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Index, sizeof(m_Index) );	

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT CGMapInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGMapInfoHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
