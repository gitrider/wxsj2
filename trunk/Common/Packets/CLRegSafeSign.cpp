#include "stdafx.h" 
#include "CLRegSafeSign.h"

BOOL CLRegSafeSign::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_UserAnswer), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLRegSafeSign::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_UserAnswer), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLRegSafeSign::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLRegSafeSignHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}
