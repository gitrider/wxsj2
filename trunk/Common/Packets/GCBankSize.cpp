// GCBankSize.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCBankSize.h"

BOOL GCBankSize::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read( (CHAR*)(&m_cSize), sizeof(BYTE));
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCBankSize::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write( (CHAR*)(&m_cSize), sizeof(BYTE));
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCBankSize::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCBankSizeHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}

