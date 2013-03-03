#include "stdafx.h" 
#include "LBAccCheck.h"


BOOL	LBAccCheck::Read(SocketBillingInputStream& iStream )
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
	iStream.Read( (CHAR*)(&mPwLength), sizeof(BYTE));
	iStream.Read( (CHAR*)(mPwMd5),sizeof(CHAR)*mPwLength);
	iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
	iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);

	iStream.Read( (CHAR*)(&mUserLevel),sizeof(USHORT));

	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		iStream.Read( (CHAR*)(mAllMiBaoKey[i]),sizeof(CHAR)*MIBAOUNIT_NAME_LENGTH);
	}
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		iStream.Read( (CHAR*)(mAllMiBaoValue[i]),sizeof(CHAR)*MIBAOUNIT_VALUE_LENGTH);
	}
	iStream.Read( (CHAR*)(mMacAddr), sizeof(CHAR)*MAX_MD5_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL	LBAccCheck::Write(SocketBillingOutputStream& oStream ) const 
{
	__ENTER_FUNCTION
		
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mPwLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mPwMd5),sizeof(CHAR)*mPwLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		USHORT UserLevel = htons(mUserLevel);
		oStream.Write( (CHAR*)(&UserLevel),sizeof(USHORT));

	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		oStream.Write( (CHAR*)(mAllMiBaoKey[i]),sizeof(CHAR)*MIBAOUNIT_NAME_LENGTH);
	}
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		oStream.Write( (CHAR*)(mAllMiBaoValue[i]),sizeof(CHAR)*MIBAOUNIT_VALUE_LENGTH);
	}
		oStream.Write( (CHAR*)(mMacAddr), sizeof(CHAR)*MAX_MD5_SIZE);
		return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

UINT LBAccCheck::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAccCheckHandler::Execute(this,pPlayer);
			
	__LEAVE_FUNCTION

		return FALSE;
}