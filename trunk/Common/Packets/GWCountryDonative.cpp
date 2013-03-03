#include "stdafx.h"
#include "GWCountryDonative.h"

BOOL GWCountryDonative::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));
        iStream.Read((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryDonative::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));
        oStream.Write((CHAR*)&m_nGold, sizeof(m_nGold));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryDonative::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryDonativeHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
