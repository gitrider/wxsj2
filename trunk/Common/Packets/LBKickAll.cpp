#include "stdafx.h" 
#include "LBKickAll.h"

BOOL LBKickAll::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION
		
		USHORT zoneid = htons(mZoneID);
		iStream.Read((CHAR*)&mZoneID,sizeof(mZoneID));
		//USHORT worldid = htons(mWorldID);
		iStream.Read((CHAR*)&mWorldID,sizeof(mWorldID));
		iStream.Read((CHAR*)&mReasonLength,sizeof(BYTE));
		iStream.Read((CHAR*)mReason,mReasonLength);
		iStream.Read((CHAR*)&mExtInfo1Length,sizeof(BYTE));
		iStream.Read((CHAR*)mExtInfo1,mExtInfo1Length);
		iStream.Read((CHAR*)&mExtInfo2Length,sizeof(BYTE));
		iStream.Read((CHAR*)mExtInfo2,mExtInfo2Length);
		iStream.Read((CHAR*)&mExtInfo3Length,sizeof(BYTE));
		iStream.Read((CHAR*)mExtInfo3,mExtInfo3Length);
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBKickAll::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

		USHORT zoneid = htons(mZoneID);
		oStream.Write((CHAR*)&zoneid,sizeof(zoneid));
		USHORT worldid = htons(mWorldID);
		oStream.Write((CHAR*)&worldid,sizeof(worldid));
		oStream.Write((CHAR*)&mReasonLength,sizeof(BYTE));
		oStream.Write((CHAR*)mReason,mReasonLength);
		oStream.Write((CHAR*)&mExtInfo1Length,sizeof(BYTE));
		oStream.Write((CHAR*)mExtInfo1,mExtInfo1Length);
		oStream.Write((CHAR*)&mExtInfo2Length,sizeof(BYTE));
		oStream.Write((CHAR*)mExtInfo2,mExtInfo2Length);
		oStream.Write((CHAR*)&mExtInfo3Length,sizeof(BYTE));
		oStream.Write((CHAR*)mExtInfo3,mExtInfo3Length);
		
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBKickAll::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBKickAllHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}
