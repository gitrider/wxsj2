#include "stdafx.h" 
#include "LWRetBillingEnd.h"


BOOL LWRetBillingEnd::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		iStream.Read((CHAR*)&mResult,sizeof(BYTE));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LWRetBillingEnd::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&mAccount,sizeof(CHAR)*(MAX_ACCOUNT+1));
		oStream.Write((CHAR*)&mResult,sizeof(BYTE));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LWRetBillingEnd::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	LWRetBillingEndHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

