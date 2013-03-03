#include "stdafx.h" 
#include "BLRetKickAll.h"

BOOL BLRetKickALL::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mResult,sizeof(mResult));
	
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetKickALL::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		oStream.Write((CHAR*)&mResult,sizeof(mResult));
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetKickALL::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetKickALLHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
