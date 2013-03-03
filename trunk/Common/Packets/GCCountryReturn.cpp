/********************************************************************************
*	�ļ�����	GCCountryReturn.cpp
*	ȫ·����	d:\Prj\Common\Packets\GCCountryReturn.cpp
*	�����ˣ�	Amos
*	����ʱ�䣺	2008 �� 4 �� 15 ��	16:28
*
*	����˵����	
*	�޸ļ�¼��
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
