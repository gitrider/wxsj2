#include "stdafx.h" 
#include "LWRetBillingStart.h"


BOOL LWRetBillingStart::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mResult,sizeof(BYTE));
		iStream.Read((CHAR*)&mFeeType,sizeof(BYTE));
		iStream.Read((CHAR*)&mLeftTime,sizeof(UINT));
		iStream.Read((CHAR*)&mStorePoint,sizeof(UINT));
		iStream.Read((CHAR*)&mUserPoint,sizeof(UINT));
		iStream.Read((CHAR*)&mCode,sizeof(BYTE));
		iStream.Read((CHAR*)&mPlayerID,sizeof(PlayerID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LWRetBillingStart::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mResult,sizeof(BYTE));
		oStream.Write((CHAR*)&mFeeType,sizeof(BYTE));
		oStream.Write((CHAR*)&mLeftTime,sizeof(UINT));
		oStream.Write((CHAR*)&mStorePoint,sizeof(UINT));
		oStream.Write((CHAR*)&mUserPoint,sizeof(UINT));
		oStream.Write((CHAR*)&mCode,sizeof(BYTE));
		oStream.Write((CHAR*)&mPlayerID,sizeof(PlayerID_t));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LWRetBillingStart::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LWRetBillingStartHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

