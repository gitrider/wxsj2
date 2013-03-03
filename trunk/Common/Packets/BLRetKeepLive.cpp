#include "stdafx.h" 
#include "BLRetKeepLive.h"

BOOL BLRetKeepLive::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mReserve,sizeof(mReserve));
	
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetKeepLive::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&mReserve,sizeof(mReserve));
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetKeepLive::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetKeepLiveHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
