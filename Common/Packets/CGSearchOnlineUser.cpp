// CGSearchOnlineUser.cpp
// 
/////////////////////////////////////////////////////

#include "stdafx.h"
#include "CGSearchOnlineUser.h"

BOOL CGSearchOnlineUser::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_PostCode), sizeof(m_PostCode) ) ;
	iStream.Read( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGSearchOnlineUser::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_PostCode), sizeof(m_PostCode) ) ;
	oStream.Write( (CHAR*)(&m_iMaxPostion), sizeof(m_iMaxPostion) ) ;


	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGSearchOnlineUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGSearchOnlineUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

