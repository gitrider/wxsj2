#include "stdafx.h"
#include "CGCountryAppoint.h"

BOOL CGCountryAppoint::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
        return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryAppoint::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
        return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryAppoint::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryAppointHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
