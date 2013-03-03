#include "stdafx.h" 
#include "BLRetAskPoint.h"



BOOL BLRetAskPoint::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
		if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
		iStream.Read(mAccount,mAccLength*sizeof(CHAR));
		iStream.Read((CHAR*)&mPoint,sizeof(UINT));
		mPoint = ntohl(mPoint);
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetAskPoint::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetAskPoint::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetAskPointHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}