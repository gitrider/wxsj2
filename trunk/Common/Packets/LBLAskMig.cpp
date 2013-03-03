#include "stdafx.h" 
#include "LBLAskMig.h"



BOOL LBLAskMig::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION

	if(BL_ASKMIG == mType)
	{
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		//USHORT ulv = htons(mCharLevel);
		iStream.Read( (CHAR*)(&mCharLevel),sizeof(USHORT));
		//UINT uGUID = htonl(mCharGUID);
		iStream.Read( (CHAR*)(&mCharGUID),sizeof(UINT));
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

BOOL LBLAskMig::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	if(LB_ASKMIG == mType)
	{
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
		USHORT ulv = htons(mCharLevel);
		oStream.Write( (CHAR*)(&ulv),sizeof(USHORT));
		UINT uGUID = htonl(mCharGUID);
		oStream.Write( (CHAR*)(&uGUID),sizeof(UINT));
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

UINT LBLAskMig::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBLAskMigHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}