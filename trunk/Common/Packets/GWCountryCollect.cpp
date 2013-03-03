#include "stdafx.h"
#include "GWCountryCollect.h"

BOOL GWCountryCollect::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryCollect::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryCollect::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryCollectHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
