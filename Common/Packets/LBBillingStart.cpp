#include "stdafx.h" 
#include "LBBillingStart.h"

BOOL LBBillingStart::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		if (mAccLength > MAX_BILLACCOUNT-1)
		{
			mAccLength = MAX_BILLACCOUNT -1;
		}
		mAccount[mAccLength] = 0;
		iStream.Read( (CHAR*)(&mCharLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),   sizeof(CHAR)*mCharLength);
	//	UINT	guid = htonl(mCharGUID);
		iStream.Read( (CHAR*)(&mCharGUID),sizeof(UINT));
	//	USHORT CharLevel = htons(mCharLevel);
		iStream.Read( (CHAR*)(&mCharLevel),sizeof(USHORT));
	//	UINT	StartTime = htonl(mStartTime);
		iStream.Read( (CHAR*)(&mStartTime),sizeof(UINT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBBillingStart::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		

	oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
	oStream.Write( (CHAR*)(&mCharLength), sizeof(BYTE));
	oStream.Write( (CHAR*)(mCharName),   sizeof(CHAR)*mCharLength);
	UINT	guid = htonl(mCharGUID);
	oStream.Write( (CHAR*)(&guid),sizeof(UINT));
	USHORT CharLevel = htons(mCharLevel);
	oStream.Write( (CHAR*)(&CharLevel),sizeof(USHORT));
	UINT	StartTime = htonl(mStartTime);
	oStream.Write( (CHAR*)(&StartTime),sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBBillingStart::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBBillingStartHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
