#include "stdafx.h" 
#include "LBLNewCheckPrize.h"



BOOL LBLNewCheckPrize::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION

	if(BL_CHECK_PRIZE == mType)
	{
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBLNewCheckPrize::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	if(LB_CHECK_PRIZE == mType)
	{
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mCharNameLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mCharName),sizeof(CHAR)*mCharNameLength);
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBLNewCheckPrize::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBLNewCheckPrizeHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}