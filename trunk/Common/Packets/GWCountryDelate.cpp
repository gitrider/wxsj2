#include "stdafx.h"
#include "GWCountryDelate.h"

BOOL GWCountryDelate::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read((CHAR*)&m_bResult, sizeof(m_bResult));
    iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryDelate::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write((CHAR*)&m_bResult, sizeof(m_bResult));
    oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryDelate::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryDelateHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
