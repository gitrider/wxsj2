#include "stdafx.h"
#include "WGCountryCollect.h"

BOOL WGCountryCollect::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL WGCountryCollect::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT WGCountryCollect::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryCollectHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
