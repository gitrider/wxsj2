#include "stdafx.h" 
#include "LBConnect.h"

BOOL LBConnect::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		//USHORT WorldID = ntohs(mWorldID);
		iStream.Read((CHAR*)&mZoneID,sizeof(mZoneID));
		iStream.Read((CHAR*)&mWorldID,sizeof(mWorldID));
		iStream.Read((CHAR*)&mIPSize,sizeof(mIPSize));
		
		if(mIPSize>MAX_BILLING_IP_SIZE) mIPSize = MAX_BILLING_IP_SIZE;

		iStream.Read(mIP,mIPSize);
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBConnect::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		USHORT zoneid = htons(mZoneID);
		oStream.Write((CHAR*)&zoneid,sizeof(zoneid));

		USHORT worldid = htons(mWorldID);
		oStream.Write((CHAR*)&worldid,sizeof(worldid));
		
		if(mIPSize>MAX_BILLING_IP_SIZE)
		{
			BYTE maxsize = MAX_BILLING_IP_SIZE;
			oStream.Write((CHAR*)&maxsize,sizeof(BYTE));
			oStream.Write(mIP,maxsize);
		}
		else
		{
			oStream.Write((CHAR*)&mIPSize,sizeof(mIPSize));
			oStream.Write(mIP,mIPSize);
		}

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBConnect::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBConnectHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
