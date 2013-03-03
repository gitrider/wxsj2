#include "stdafx.h"
#include "GWCountryRemove.h"

BOOL GWCountryRemove::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryRemove::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryRemove::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryRemoveHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
