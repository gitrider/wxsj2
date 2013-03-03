#include "stdafx.h"
#include "WGCountryInfoConfirm.h"

BOOL WGCountryInfoConfirm::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)(&m_PlayerID), sizeof(m_PlayerID));
        iStream.Read((CHAR*)(&m_Info), sizeof(COUNTRY_CGW_COUNTRY_INFO));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL WGCountryInfoConfirm::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        
        oStream.Write((CHAR*)(&m_PlayerID), sizeof(m_PlayerID));
        oStream.Write((CHAR*)(&m_Info), sizeof(COUNTRY_CGW_COUNTRY_INFO));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT WGCountryInfoConfirm::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryInfoConfirmHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
