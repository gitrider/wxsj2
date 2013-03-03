#include "stdafx.h" 
#include "LBBillingKeep.h"

BOOL LBBillingKeep::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
	//	USHORT CharLevel = htons(mCharLevel);
		iStream.Read( (CHAR*)(&mCharLevel),sizeof(USHORT));
	//	UINT	StartTime = htonl(mStartTime);
		iStream.Read( (CHAR*)(&mStartTime),sizeof(UINT));
	//	UINT	EndTime = htonl(mEndTime);
		iStream.Read( (CHAR*)(&mEndTime),sizeof(UINT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBBillingKeep::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
	USHORT CharLevel = htons(mCharLevel);
	oStream.Write( (CHAR*)(&CharLevel),sizeof(USHORT));
	UINT	StartTime = htonl(mStartTime);
	oStream.Write( (CHAR*)(&StartTime),sizeof(UINT));
	UINT	EndTime = htonl(mEndTime);
	oStream.Write( (CHAR*)(&EndTime),sizeof(UINT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBBillingKeep::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBBillingKeepHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
