#include "stdafx.h"
#include "GWCountryInfoRequest.h"

BOOL GWCountryInfoRequest::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)(&m_Guid), sizeof(m_Guid));
        iStream.Read((CHAR*)(&m_nCountryID), sizeof(m_nCountryID));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryInfoRequest::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)(&m_Guid), sizeof(m_Guid));
        oStream.Write((CHAR*)(&m_nCountryID), sizeof(m_nCountryID));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryInfoRequest::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryInfoRequestHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
