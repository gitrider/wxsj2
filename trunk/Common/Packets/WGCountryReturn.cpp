/********************************************************************************
*	文件名：	WGCountryReturn.cpp
*	全路径：	d:\Prj\Common\Packets\WGCountryReturn.cpp
*	创建人：	Amos
*	创建时间：	2008 年 4 月 15 日	16:11
*
*	功能说明：	
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "WGCountryReturn.h"

BOOL	WGCountryReturn::Read(SocketInputStream& iStream )
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
    m_CountryReturn.Read(iStream);

    return TRUE;

    __LEAVE_FUNCTION

    return FALSE;
}

BOOL	WGCountryReturn::Write(SocketOutputStream& oStream )	const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
    m_CountryReturn.Write(oStream);

    return TRUE;

    __LEAVE_FUNCTION

    return FALSE;
}


UINT	WGCountryReturn::Execute(Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryReturnHandler::Execute(this, pPlayer);

    __LEAVE_FUNCTION

        return FALSE;
}
