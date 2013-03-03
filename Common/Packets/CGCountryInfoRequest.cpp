#include "stdafx.h"
#include "CGCountryInfoRequest.h"

BOOL CGCountryInfoRequest::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read((CHAR*)(&m_nCountryID), sizeof(m_nCountryID));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryInfoRequest::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write((CHAR*)(&m_nCountryID), sizeof(m_nCountryID));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryInfoRequest::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryInfoRequestHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
