#include "stdafx.h" 
#include "BLRetBillingStart.h"

BOOL BLRetBillingStart::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
	iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
	if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
	iStream.Read(mAccount,mAccLength*sizeof(CHAR));
	iStream.Read((CHAR*)&mResult,sizeof(BYTE));

	if(mResult==0x01)
	{
		iStream.Read((CHAR*)&mFeeType,sizeof(BYTE));
		iStream.Read((CHAR*)&mLeftTime,sizeof(UINT));
		mLeftTime = ntohl(mLeftTime);
		iStream.Read((CHAR*)&mStorePoint,sizeof(UINT));
		mStorePoint = ntohl(mStorePoint);
		iStream.Read((CHAR*)&mUserPoint,sizeof(UINT));
		mUserPoint = ntohl(mUserPoint);		
	}
	else if(mResult==0x00)
	{
		iStream.Read((CHAR*)&mCode,sizeof(BYTE));
	}

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetBillingStart::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
	oStream.Write((CHAR*)&mAccLength,sizeof(BYTE));
	oStream.Write(mAccount,mAccLength*sizeof(CHAR));
	oStream.Write((CHAR*)&mResult,sizeof(BYTE));

	if(mResult==0x01)
	{
		oStream.Write((CHAR*)&mFeeType,sizeof(BYTE));
		oStream.Write((CHAR*)&mLeftTime,sizeof(UINT));
		//mLeftTime = ntohl(mLeftTime);
		oStream.Write((CHAR*)&mStorePoint,sizeof(UINT));
		//mStorePoint = ntohl(mStorePoint);
		oStream.Write((CHAR*)&mUserPoint,sizeof(UINT));
		//mUserPoint = ntohl(mUserPoint);		
	}
	else if(mResult==0x00)
	{
		oStream.Write((CHAR*)&mCode,sizeof(BYTE));
	}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetBillingStart::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetBillingStartHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
