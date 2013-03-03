#include "stdafx.h" 
#include "LBLNewPrize.h"



BOOL LBLNewPrize::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION

	if(BL_NEW_PRIZE == mType)
	{
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		iStream.Read( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBLNewPrize::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	if(LB_NEW_PRIZE == mType)
	{
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		oStream.Write( (CHAR*)(mPrizeSerial),sizeof(CHAR)*MAX_PRIZE_SERIAL_LENGTH);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBLNewPrize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBLNewPrizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}