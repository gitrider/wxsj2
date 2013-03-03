#include "stdafx.h"
#include "GWCountryRequestKindom.h"

BOOL GWCountryRequestKindom::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GWCountryRequestKindom::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GWCountryRequestKindom::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GWCountryRequestKindomHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
