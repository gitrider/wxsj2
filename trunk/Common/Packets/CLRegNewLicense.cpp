#include "stdafx.h" 
#include "CLRegNewLicense.h"

BOOL CLRegNewLicense::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(&m_UserAnswer), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CLRegNewLicense::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(&m_UserAnswer), sizeof(BYTE));
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLRegNewLicense::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLRegNewLicenseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}
