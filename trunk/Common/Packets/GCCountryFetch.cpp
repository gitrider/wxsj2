#include "stdafx.h"
#include "GCCountryFetch.h"

BOOL GCCountryFetch::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCCountryFetch::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCCountryFetch::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCCountryFetchHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
