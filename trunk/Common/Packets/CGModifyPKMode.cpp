#include "stdafx.h"
#include "CGModifyPKMode.h"

BOOL CGModifyPKMode::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&mCampMode), sizeof(BYTE));
	 return TRUE;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL CGModifyPKMode::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
	
		oStream.Write( (CHAR*)(&mCampMode), sizeof(BYTE));
		return TRUE;

	__LEAVE_FUNCTION
		return FALSE ;
}

UINT	CGModifyPKMode::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return CGModifyPKModeHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}