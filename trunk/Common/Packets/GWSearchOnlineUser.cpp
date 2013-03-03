// GWSearchOnlineUser.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GWSearchOnlineUser.h"

BOOL GWSearchOnlineUser::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	iStream.Read( (CHAR*)(&m_PostCode), sizeof(m_PostCode) ) ;
	iStream.Read( (CHAR*)(&m_GUID), sizeof(m_GUID) ) ;
	iStream.Read( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion) ) ;
	
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWSearchOnlineUser::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	oStream.Write( (CHAR*)(&m_PostCode), sizeof(m_PostCode) ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID) );
	oStream.Write( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion) );

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWSearchOnlineUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWSearchOnlineUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

