#include "stdafx.h"
#include "CGCountryDelate.h"

BOOL CGCountryDelate::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)&m_bResult, sizeof(m_bResult));
        iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryDelate::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)&m_bResult, sizeof(m_bResult));
        oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryDelate::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryDelateHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
