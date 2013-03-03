#include "stdafx.h" 
#include "WLBillingEnd.h"


BOOL WLBillingEnd::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mCharLevel,sizeof(USHORT));
		iStream.Read((CHAR*)&mEndType,sizeof(BYTE));
		iStream.Read((CHAR*)&mStartTime,sizeof(UINT));
		iStream.Read((CHAR*)&mEndTime,sizeof(UINT));
	
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WLBillingEnd::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mCharLevel,sizeof(USHORT));
		oStream.Write((CHAR*)&mEndType,sizeof(BYTE));
		oStream.Write((CHAR*)&mStartTime,sizeof(UINT));
		oStream.Write((CHAR*)&mEndTime,sizeof(UINT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WLBillingEnd::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	WLBillingEndHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

