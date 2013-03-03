#include "stdafx.h" 
#include "BLRetBillingKeep.h"

BOOL BLRetBillingKeep::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
	iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
	if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
	iStream.Read(mAccount,mAccLength*sizeof(CHAR));
	iStream.Read((CHAR*)&mResult,sizeof(BYTE));
	if(mResult==0x01)
	{
	
		iStream.Read((CHAR*)&mLeftTime,sizeof(UINT));
		mLeftTime = ntohl(mLeftTime);
		iStream.Read((CHAR*)&mStorePoint,sizeof(UINT));
		mStorePoint = ntohl(mStorePoint);
		iStream.Read((CHAR*)&mUserPoint,sizeof(UINT));
		mUserPoint = ntohl(mUserPoint);		
	}
	
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetBillingKeep::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetBillingKeep::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetBillingKeepHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
