

#include "stdafx.h"
#include "WGOperateResult.h"



BOOL WGOperateResult::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)(&m_nResult), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGOperateResult::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)(&m_nResult), sizeof(INT) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGOperateResult::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGOperateResultHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


