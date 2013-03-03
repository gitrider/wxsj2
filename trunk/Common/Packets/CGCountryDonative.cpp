#include "stdafx.h"
#include "CGCountryDonative.h"

BOOL CGCountryDonative::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryDonative::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));
        
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryDonative::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryDonativeHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
