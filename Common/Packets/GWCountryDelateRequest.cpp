#include "stdafx.h"
#include "GWCountryDelateRequest.h"

BOOL GWCountryDelateRequest::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryDelateRequest::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryDelateRequest::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryDelateRequestHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
