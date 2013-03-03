#include "stdafx.h"
#include "GWCountryAP.h"

BOOL GWCountryAP::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read((CHAR*)&m_Guid, sizeof(m_Guid));
    iStream.Read((CHAR*)&m_cType, sizeof(m_cType));
    iStream.Read((CHAR*)&m_nAP, sizeof(m_nAP));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryAP::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write((CHAR*)&m_Guid, sizeof(m_Guid));
    oStream.Write((CHAR*)&m_cType, sizeof(m_cType));
    oStream.Write((CHAR*)&m_nAP, sizeof(m_nAP));

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryAP::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryAPHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
