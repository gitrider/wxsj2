#include "stdafx.h" 
#include "LBAskPoint.h"



BOOL LBAskPoint::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBAskPoint::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBAskPoint::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskPointHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}