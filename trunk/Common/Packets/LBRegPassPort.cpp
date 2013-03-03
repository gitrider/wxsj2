#include "stdafx.h" 
#include "LBRegPassPort.h"



BOOL LBRegPassPort::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(&mAccLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		iStream.Read( (CHAR*)(&mSuperPwLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mSuperPw),   sizeof(CHAR)*mSuperPwLength);
		iStream.Read( (CHAR*)(&mPwLength), sizeof(BYTE));
		iStream.Read( (CHAR*)(mPwMd5), sizeof(CHAR)*mPwLength);
		iStream.Read( (CHAR*)(&mIPLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		iStream.Read( (CHAR*)(&mEmailLength),sizeof(BYTE));
		iStream.Read( (CHAR*)(mEmail),sizeof(CHAR)*mEmailLength);

		//USHORT	oL = htons(mOtherInfo1Length);
		iStream.Read( (CHAR*)(&mOtherInfo1Length), sizeof(USHORT));
		iStream.Read( (CHAR*)(mOtherInfo1), sizeof(CHAR)*mOtherInfo1Length);
		//oL = htons(mOtherInfo2Length);
		iStream.Read( (CHAR*)(&mOtherInfo2Length), sizeof(USHORT));
		iStream.Read( (CHAR*)(mOtherInfo2), sizeof(CHAR)*mOtherInfo2Length);
		//oL = htons(mOtherInfo3Length);
		iStream.Read( (CHAR*)(&mOtherInfo3Length), sizeof(USHORT));
		iStream.Read( (CHAR*)(mOtherInfo3), sizeof(CHAR)*mOtherInfo3Length);
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL LBRegPassPort::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&mAccLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mAccount),   sizeof(CHAR)*mAccLength);
		oStream.Write( (CHAR*)(&mSuperPwLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mSuperPw),   sizeof(CHAR)*mSuperPwLength);
		oStream.Write( (CHAR*)(&mPwLength), sizeof(BYTE));
		oStream.Write( (CHAR*)(mPwMd5), sizeof(CHAR)*mPwLength);
		oStream.Write( (CHAR*)(&mIPLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mIP),sizeof(CHAR)*mIPLength);
		oStream.Write( (CHAR*)(&mEmailLength),sizeof(BYTE));
		oStream.Write( (CHAR*)(mEmail),sizeof(CHAR)*mEmailLength);

		USHORT	oL = htons(mOtherInfo1Length);
		oStream.Write( (CHAR*)(&oL), sizeof(USHORT));
		oStream.Write( (CHAR*)(mOtherInfo1), sizeof(CHAR)*mOtherInfo1Length);
		oL = htons(mOtherInfo2Length);
		oStream.Write( (CHAR*)(&oL), sizeof(USHORT));
		oStream.Write( (CHAR*)(mOtherInfo2), sizeof(CHAR)*mOtherInfo2Length);
		oL = htons(mOtherInfo3Length);
		oStream.Write( (CHAR*)(&oL), sizeof(USHORT));
		oStream.Write( (CHAR*)(mOtherInfo3), sizeof(CHAR)*mOtherInfo3Length);
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT LBRegPassPort::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBRegPassPortHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}