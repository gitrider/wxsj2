#include "stdafx.h" 
#include "WLBillingKeep.h"

BOOL WLBillingKeep::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mCharLevel,sizeof(SHORT));
		iStream.Read((CHAR*)&mStartTime,sizeof(UINT));
		iStream.Read((CHAR*)&mEndTime,sizeof(UINT));

		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WLBillingKeep::Write( SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mCharLevel,sizeof(SHORT));
		oStream.Write((CHAR*)&mStartTime,sizeof(UINT));
		oStream.Write((CHAR*)&mEndTime,sizeof(UINT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WLBillingKeep::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WLBillingKeepHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
