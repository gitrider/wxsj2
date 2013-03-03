/********************************************************************************
*	�ļ�����	WGCountryReturn.cpp
*	ȫ·����	d:\Prj\Common\Packets\WGCountryReturn.cpp
*	�����ˣ�	Amos
*	����ʱ�䣺	2008 �� 4 �� 15 ��	16:11
*
*	����˵����	
*	�޸ļ�¼��
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
