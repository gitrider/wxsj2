#include "stdafx.h" 
#include "LWNotifyUser.h"
#include "DB_Struct.h"

BOOL LWNotifyUser::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_AccName), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_NotifyStatus), sizeof(LOGIN_NOTIFY_STATUS));
	iStream.Read((CHAR*)(&m_uKey), sizeof(UINT));
	
	iStream.Read((CHAR*)(&m_CharGUID), sizeof(GUID_t));
	iStream.Read((CHAR*)(&m_SaveCrc), sizeof(UINT));
	iStream.Read((CHAR*)(&m_LoadCrc), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL  LWNotifyUser::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_AccName), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_NotifyStatus), sizeof(LOGIN_NOTIFY_STATUS));
	oStream.Write((CHAR*)(&m_uKey), sizeof(UINT));

	oStream.Write((CHAR*)(&m_CharGUID), sizeof(GUID_t));
	oStream.Write((CHAR*)(&m_SaveCrc), sizeof(UINT));
	oStream.Write((CHAR*)(&m_LoadCrc), sizeof(UINT));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT  LWNotifyUser::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LWNotifyUserHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const	CHAR*	LWNotifyUser::GetAccount()	const
{
	return m_AccName;
}
VOID			LWNotifyUser::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(m_AccName,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	m_AccName[MAX_ACCOUNT] = '\0' ;
}
