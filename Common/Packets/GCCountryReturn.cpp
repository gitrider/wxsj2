/********************************************************************************
*	文件名：	GCCountryReturn.cpp
*	全路径：	d:\Prj\Common\Packets\GCCountryReturn.cpp
*	创建人：	Amos
*	创建时间：	2008 年 4 月 15 日	16:28
*
*	功能说明：	
*	修改记录：
*********************************************************************************/

#include "stdafx.h"
#include "GCCountryReturn.h"

BOOL	GCCountryReturn::Read(SocketInputStream& iStream )
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
    m_CountryReturn.Read(iStream);

    return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}

BOOL	GCCountryReturn::Write(SocketOutputStream& oStream )	const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
    m_CountryReturn.Write(oStream);

    return TRUE;

    __LEAVE_FUNCTION

        return FALSE;
}


UINT	GCCountryReturn::Execute(Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCCountryReturnHandler::Execute(this, pPlayer);

    __LEAVE_FUNCTION

        return FALSE;
}
