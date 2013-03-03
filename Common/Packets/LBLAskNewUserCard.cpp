#include "stdafx.h" 
#include "LBLAskNewUserCard.h"



BOOL LBLAskNewUserCard::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION
	if(BL_NEWUSERCARD == mType)
	{
		iStream.Read( (CHAR*)(&mCardLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mNewuserCard),sizeof(CHAR)*mCardLength);
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

BOOL LBLAskNewUserCard::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	if(LB_NEWUSERCARD == mType)
	{
		oStream.Write( (CHAR*)(&mCardLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mNewuserCard),sizeof(CHAR)*mCardLength);
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

UINT LBLAskNewUserCard::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBLAskNewUserCardHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}