

#include "stdafx.h"
#include "CGCharUpdateCurTitle.h"


BOOL CGCharUpdateCurTitle::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_TitleType), sizeof(BYTE) ) ;
	iStream.Read( (CHAR*)(&m_TitleID), sizeof(INT) ) ;
    iStream.Read( (CHAR*)(&m_Guid), sizeof(GUID_t) ) ;
    iStream.Read( (CHAR*)(&m_cTitleSize), sizeof(BYTE) ) ;
    if (m_cTitleSize>0)
    {
        iStream.Read( (CHAR*)m_szTitleName, m_cTitleSize ) ;
    }	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CGCharUpdateCurTitle::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

    oStream.Write( (CHAR*)(&m_TitleType), sizeof(BYTE) ) ;
    oStream.Write( (CHAR*)(&m_TitleID), sizeof(INT) ) ;
    oStream.Write( (CHAR*)(&m_Guid), sizeof(GUID_t) ) ;
    oStream.Write( (CHAR*)(&m_cTitleSize), sizeof(BYTE) ) ;
    if (m_cTitleSize>0)
    {
        oStream.Write( (CHAR*)m_szTitleName, m_cTitleSize ) ;
    }

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CGCharUpdateCurTitle::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGCharUpdateCurTitleHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


