/********************************************************************************
 *	文件名：	GWTopList.h
 *	创建人：	王勇鸷
 *	创建时间：	2008 年 4 月 10 日
 *
 *	功能说明：	服务器端往 World 发送的排行榜相关的操作请求
 *	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GWTopList.h"

BOOL GWTopList::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_GUID, sizeof(m_GUID));
	iStream.Read((CHAR*)&m_Type, sizeof(m_Type));
	return TRUE;

__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL GWTopList::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_GUID, sizeof(m_GUID));
	oStream.Write((CHAR*)&m_Type, sizeof(m_Type));
	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GWTopList::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWTopListHandler::Execute( this, pPlayer );

__LEAVE_FUNCTION

	return FALSE;
}
