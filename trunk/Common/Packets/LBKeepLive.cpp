#include "stdafx.h" 
#include "LBKeepLive.h"

BOOL LBKeepLive::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		iStream.Read((CHAR*)&mWorldID,sizeof(mZoneID));
		iStream.Read((CHAR*)&mWorldID,sizeof(mWorldID));
		iStream.Read((CHAR*)&mPlayerCount,sizeof(mPlayerCount));
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBKeepLive::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		USHORT zoneid = htons(mZoneID);
		oStream.Write((CHAR*)&zoneid,sizeof(zoneid));

		USHORT worldid = htons(mWorldID);

		oStream.Write((CHAR*)&worldid,sizeof(worldid));
		
		USHORT playercount = htons(mPlayerCount);

		oStream.Write((CHAR*)&playercount,sizeof(playercount));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBKeepLive::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBKeepLiveHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
