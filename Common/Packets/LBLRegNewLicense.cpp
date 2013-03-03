#include "stdafx.h" 
#include "LBLRegNewLicense.h"



BOOL LBLRegNewLicense::Read( SocketBillingInputStream& iStream ) 
{
__ENTER_FUNCTION

	if(BL_REGLICENSE == mType)
	{
		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read((CHAR*)&mRegLicense,sizeof(UCHAR));
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBLRegNewLicense::Write( SocketBillingOutputStream& oStream )	const
{
__ENTER_FUNCTION
	if(LB_REGLICENSE == mType)
	{
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write((CHAR*)&mRegLicense,sizeof(UCHAR));
	}
	else
	{
		Assert(FALSE);
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBLRegNewLicense::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBLRegNewLicenseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}