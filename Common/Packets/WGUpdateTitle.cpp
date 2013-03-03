/********************************************************************************
*	文件名：	WGUpdateTitle.CPP
*	全路径：	d:\Prj\Common\Packets\WGUpdateTitle.CPP
*	创建人：	Amos
*	创建时间：	2008 年 3 月 28 日	
*
*	功能说明：	处理其他玩家修改自身称号名字
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGUpdateTitle.h"

using namespace Packets;

BOOL WGUpdateTitle::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
    iStream.Read( (CHAR*)(&m_uTitleSize), sizeof(UCHAR) );
    iStream.Read( (CHAR*)(&m_TitleType), sizeof(BYTE) );
    iStream.Read( (CHAR*)(&m_TitleID), sizeof(INT) );

    if( m_uTitleSize > 0 )
    {
        iStream.Read( m_szTitle, sizeof(CHAR) * m_uTitleSize );
    }

    return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}

BOOL WGUpdateTitle::Write( SocketOutputStream& oStream ) const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) );
    oStream.Write( (CHAR*)(&m_uTitleSize), sizeof(UCHAR) );
    oStream.Write( (CHAR*)(&m_TitleType), sizeof(BYTE) );
    oStream.Write( (CHAR*)(&m_TitleID), sizeof(INT) );

    if( m_uTitleSize > 0 )
    {
        oStream.Write( m_szTitle, sizeof(CHAR) * m_uTitleSize );
    }

    return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}

UINT WGUpdateTitle::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGUpdateTitleHandler::Execute( this, pPlayer );

    __LEAVE_FUNCTION

        return FALSE;
}