

#include "stdafx.h"
#include "WGMail.h"



BOOL WGMail::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

 	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	m_MailList.Read( iStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGMail::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

 	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t) ) ;
	m_MailList.Write( oStream ) ;
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGMail::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WGMailHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


