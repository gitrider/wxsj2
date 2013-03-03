// GCReturnOnlineUser.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "GCReturnOnlineUser.h"

BOOL GCReturnOnlineUser::Read( SocketInputStream& iStream ) 
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

BOOL GCReturnOnlineUser::Write( SocketOutputStream& oStream )const
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

UINT GCReturnOnlineUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCReturnOnlineUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

