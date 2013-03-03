#include "stdafx.h"
#include "CGCountryRemove.h"

BOOL CGCountryRemove::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryRemove::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryRemove::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryRemoveHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
