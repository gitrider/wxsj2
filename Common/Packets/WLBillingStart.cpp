#include "stdafx.h" 
#include "WLBillingStart.h"


BOOL WLBillingStart::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mCharName,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mCharGUID,sizeof(GUID_t));
		iStream.Read((CHAR*)&mCharLevel,sizeof(SHORT));
		iStream.Read((CHAR*)&mStartTime,sizeof(UINT));
		iStream.Read((CHAR*)&mPlayerID,sizeof(PlayerID_t));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WLBillingStart::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mCharName,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mCharGUID,sizeof(GUID_t));
		oStream.Write((CHAR*)&mCharLevel,sizeof(SHORT));
		oStream.Write((CHAR*)&mStartTime,sizeof(UINT));
		oStream.Write((CHAR*)&mPlayerID,sizeof(PlayerID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WLBillingStart::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	WLBillingStartHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

