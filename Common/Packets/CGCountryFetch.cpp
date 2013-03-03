#include "stdafx.h"
#include "CGCountryFetch.h"

BOOL CGCountryFetch::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryFetch::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryFetch::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryFetchHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
