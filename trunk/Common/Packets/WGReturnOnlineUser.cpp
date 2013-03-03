// WGReturnOnlineUser.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "WGReturnOnlineUser.h"

BOOL WGReturnOnlineUser::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	iStream.Read( (CHAR*)(&m_bContinue), sizeof(m_bContinue) ) ;
	m_ReturnOnlineUsers.Read(iStream);
	iStream.Read( (CHAR*)(&m_iReturnPostion), sizeof(m_iReturnPostion) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGReturnOnlineUser::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PlayerID), sizeof(m_PlayerID) ) ;
	oStream.Write( (CHAR*)(&m_bContinue), sizeof(m_bContinue) ) ;
	m_ReturnOnlineUsers.Write(oStream);	
	oStream.Write( (CHAR*)(&m_iReturnPostion), sizeof(m_iReturnPostion) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGReturnOnlineUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGReturnOnlineUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

