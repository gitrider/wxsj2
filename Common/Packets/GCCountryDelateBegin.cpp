#include "stdafx.h"
#include "GCCountryDelateBegin.h"

BOOL GCCountryDelateBegin::Read( SocketInputStream& iStream ) 
{
    __ENTER_FUNCTION

    iStream.Read( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
    iStream.Read( (CHAR*)(&m_cNameSize), sizeof(m_cNameSize) );
    if (m_cNameSize>0)
    {
        iStream.Read( (CHAR*)m_szKindomName, m_cNameSize );
    }
   

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

BOOL GCCountryDelateBegin::Write( SocketOutputStream& oStream )const
{
    __ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_cCurNum), sizeof(m_cCurNum) );
    oStream.Write( (CHAR*)(&m_cNameSize), sizeof(m_cNameSize) );
    if (m_cNameSize>0)
    {
        oStream.Write( (CHAR*)m_szKindomName, m_cNameSize );
    }

    return TRUE ;

    __LEAVE_FUNCTION

        return FALSE ;
}

UINT GCCountryDelateBegin::Execute( Player* pPlayer )
{
    __ENTER_FUNCTION

        return GCCountryDelateBeginHandler::Execute( this, pPlayer ) ;

    __LEAVE_FUNCTION

        return FALSE ;
}
