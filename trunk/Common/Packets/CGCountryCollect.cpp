#include "stdafx.h"
#include "CGCountryCollect.h"

BOOL CGCountryCollect::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        m_Data.Read(iStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL CGCountryCollect::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        m_Data.Write(oStream);
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT CGCountryCollect::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

		return CGCountryCollectHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
