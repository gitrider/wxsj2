#include "stdafx.h"
#include "CGCountryRequestKindom.h"

BOOL CGCountryRequestKindom::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
        return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryRequestKindom::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
        return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryRequestKindom::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return CGCountryRequestKindomHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
