#include "stdafx.h"
#include "GCCountryCollect.h"

BOOL GCCountryCollect::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCCountryCollect::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCCountryCollect::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCCountryCollectHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
