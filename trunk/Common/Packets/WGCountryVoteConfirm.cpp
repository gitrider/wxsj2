#include "stdafx.h"
#include "WGCountryVoteConfirm.h"

BOOL WGCountryVoteConfirm::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        iStream.Read( (CHAR*)(&m_Guid), sizeof(m_Guid) );
        iStream.Read( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
        iStream.Read( (CHAR*)(&m_nLeftTime), sizeof(m_nLeftTime) );
        iStream.Read( (CHAR*)&m_nPercent, sizeof(m_nPercent) );
        

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL WGCountryVoteConfirm::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        oStream.Write( (CHAR*)(&m_Guid), sizeof(m_Guid) );
        oStream.Write( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
        oStream.Write( (CHAR*)(&m_nLeftTime), sizeof(m_nLeftTime) );
        oStream.Write( (CHAR*)&m_nPercent, sizeof(m_nPercent) );

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT WGCountryVoteConfirm::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryVoteConfirmHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
