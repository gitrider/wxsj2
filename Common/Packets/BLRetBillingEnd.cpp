#include "stdafx.h" 
#include "BLRetBillingEnd.h"

BOOL BLRetBillingEnd::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
	iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
	if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
	iStream.Read(mAccount,mAccLength*sizeof(CHAR));
	iStream.Read((CHAR*)&mResult,sizeof(BYTE));

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL BLRetBillingEnd::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetBillingEnd::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetBillingEndHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
