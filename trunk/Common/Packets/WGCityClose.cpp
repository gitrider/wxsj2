#include "stdafx.h"
#include "WGCityClose.h"

BOOL WGCityClose::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_SceneID), sizeof(SceneID_t));
	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGCityClose::Write( SocketOutputStream& oStream ) const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_SceneID), sizeof(SceneID_t));
	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(PlayerID_t));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGCityClose::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGCityCloseHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


