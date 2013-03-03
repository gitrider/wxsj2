#include "stdafx.h"
#include "GWCountryVoteRequest.h"

BOOL GWCountryVoteRequest::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryVoteRequest::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryVoteRequest::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryVoteRequestHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
