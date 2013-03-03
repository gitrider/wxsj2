#include "stdafx.h" 
#include "LBPrize.h"



BOOL LBPrize::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		//USHORT nLevel = htons(mLevel);
		iStream.Read( (CHAR*)(&mLevel),sizeof(USHORT));
		iStream.Read( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		//USHORT nType = htons(mPrizeSerial);
		iStream.Read( (CHAR*)(&mPrizeSerial),sizeof(USHORT));
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBPrize::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		USHORT nLevel = htons(mLevel);
		oStream.Write( (CHAR*)(&nLevel),sizeof(USHORT));
		oStream.Write( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
		USHORT nType = htons(mPrizeType);
		oStream.Write( (CHAR*)(&nType),sizeof(USHORT));

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBPrize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBPrizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}