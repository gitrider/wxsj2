#include "stdafx.h"
#include "GWCountryAppoint.h"

BOOL GWCountryAppoint::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryAppoint::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryAppoint::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryAppointHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
