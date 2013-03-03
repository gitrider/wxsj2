
#include "stdafx.h" 
#include "BLRetRegPassPort.h"



BOOL BLRetRegPassPort::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
		if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
		iStream.Read(mAccount,mAccLength*sizeof(CHAR));
		iStream.Read((CHAR*)&mAccResult,sizeof(BYTE));
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetRegPassPort::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetRegPassPort::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetRegPassPortHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}