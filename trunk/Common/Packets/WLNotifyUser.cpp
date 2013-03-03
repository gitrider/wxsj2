#include "stdafx.h" 
#include "WLNotifyUser.h"

BOOL WLNotifyUser::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read((CHAR*)&m_AccName,sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)&m_NotifyStatus,sizeof(WORLD_NOTIFY_STATUS));
	iStream.Read((CHAR*)(&m_uKey), sizeof(UINT));
	
	return TRUE ;

__LEAVE_FUNCTION
	return FALSE ;
}

BOOL WLNotifyUser::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write((CHAR*)&m_AccName,sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)&m_NotifyStatus,sizeof(WORLD_NOTIFY_STATUS));
	oStream.Write((CHAR*)(&m_uKey), sizeof(UINT));

	return TRUE ;

__LEAVE_FUNCTION
	return FALSE ;
}

UINT WLNotifyUser::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return	WLNotifyUserHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION
	return FALSE ;
}


