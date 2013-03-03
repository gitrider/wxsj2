// GCExpandBankSize.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCExpandBankSize.h"

BOOL GCExpandBankSize::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_cSize), sizeof(BYTE));
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCExpandBankSize::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_cSize), sizeof(BYTE));
    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCExpandBankSize::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCExpandBankSizeHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}

