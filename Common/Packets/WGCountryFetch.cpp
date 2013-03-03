#include "stdafx.h"
#include "WGCountryFetch.h"

BOOL WGCountryFetch::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL WGCountryFetch::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT WGCountryFetch::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryFetchHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
