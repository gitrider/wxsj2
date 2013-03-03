#include "stdafx.h" 
#include "BLRetConnect.h"

BOOL BLRetConnect::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mReserve,sizeof(mReserve));
	
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetConnect::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&mReserve,sizeof(mReserve));
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetConnect::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetConnectHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
