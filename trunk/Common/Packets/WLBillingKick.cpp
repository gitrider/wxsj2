#include "stdafx.h" 
#include "WLBillingKick.h"


BOOL WLBillingKick::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read((CHAR*)&mWorldID,sizeof(USHORT));
		return TRUE ;

	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL WLBillingKick::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		oStream.Write((CHAR*)&mWorldID,sizeof(USHORT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT WLBillingKick::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return	WLBillingKickHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

