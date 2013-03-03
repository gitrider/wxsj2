#include "stdafx.h"
#include "WGCountryDelateConfirm.h"

BOOL WGCountryDelateConfirm::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

        iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        iStream.Read( (CHAR*)(&m_Guid), sizeof(m_Guid) );
        iStream.Read( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
        iStream.Read( (CHAR*)(&m_cNameSize), sizeof(m_cNameSize) );
        if (m_cNameSize > 0)
        {
            iStream.Read( (CHAR*)m_szKindomName, m_cNameSize );
        }
        

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL WGCountryDelateConfirm::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

        oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) );
        oStream.Write( (CHAR*)(&m_Guid), sizeof(m_Guid) );
        oStream.Write( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
        oStream.Write( (CHAR*)(&m_cNameSize), sizeof(m_cNameSize) );
        if (m_cNameSize > 0)
        {
            oStream.Write( (CHAR*)m_szKindomName, m_cNameSize );
        }

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT WGCountryDelateConfirm::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return WGCountryDelateConfirmHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
