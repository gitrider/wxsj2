#include "stdafx.h" 
#include "LWRetBillingKeep.h"

BOOL LWRetBillingKeep::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mResult,sizeof(BYTE));
		iStream.Read((CHAR*)&mLeftTime,sizeof(UINT));
		iStream.Read((CHAR*)&mStorePoint,sizeof(UINT));
		iStream.Read((CHAR*)&mUserPoint,sizeof(UINT));

		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LWRetBillingKeep::Write( SocketOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mResult,sizeof(BYTE));
		oStream.Write((CHAR*)&mLeftTime,sizeof(UINT));
		oStream.Write((CHAR*)&mStorePoint,sizeof(UINT));
		oStream.Write((CHAR*)&mUserPoint,sizeof(UINT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LWRetBillingKeep::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LWRetBillingKeepHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
