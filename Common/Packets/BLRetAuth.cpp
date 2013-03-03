#include "stdafx.h" 
#include "BLRetAuth.h"


BOOL BLRetAuth::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read((CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	iStream.Read((CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read((CHAR*)(&PlayerGUID), sizeof(GUID_t));
	iStream.Read((CHAR*)mServerName,sizeof(CHAR)*MAX_SERVER_NAME);
	iStream.Read((CHAR*)mIP,sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	iStream.Read((CHAR*)(&mIsFatigue), sizeof(CHAR));
	iStream.Read((CHAR*)(&mAccTotalOnlineSecond), sizeof(UINT));
	iStream.Read((CHAR*)&mIsPhoneBind,sizeof(CHAR));
	iStream.Read((CHAR*)&mIsIPBind,sizeof(CHAR));
	iStream.Read((CHAR*)&mIsMiBaoBind,sizeof(CHAR));
	iStream.Read((CHAR*)&mIsMacBind,sizeof(CHAR));
	iStream.Read((CHAR*)&mIsRealNameBind,sizeof(CHAR));
	iStream.Read((CHAR*)&mIsInputNameBind,sizeof(CHAR));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetAuth::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)(&szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write((CHAR*)(&Result),sizeof(LOGIN_RESULT));
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	oStream.Write((CHAR*)(&PlayerGUID),sizeof(GUID_t));
	oStream.Write((CHAR*)mServerName,sizeof(CHAR)*MAX_SERVER_NAME);
	oStream.Write((CHAR*)mIP,sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	oStream.Write((CHAR*)(&mIsFatigue), sizeof(CHAR));
	oStream.Write((CHAR*)(&mAccTotalOnlineSecond), sizeof(UINT));
	oStream.Write((CHAR*)&mIsPhoneBind,sizeof(CHAR));
	oStream.Write((CHAR*)&mIsIPBind,sizeof(CHAR));
	oStream.Write((CHAR*)&mIsMiBaoBind,sizeof(CHAR));
	oStream.Write((CHAR*)&mIsMacBind,sizeof(CHAR));
	oStream.Write((CHAR*)&mIsRealNameBind,sizeof(CHAR));
	oStream.Write((CHAR*)&mIsInputNameBind,sizeof(CHAR));

	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetAuth::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetAuthHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	BLRetAuth::GetAccount()	const
{
	return szAccount;
}
VOID			BLRetAuth::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = '\0' ;
}

LOGIN_RESULT	BLRetAuth::GetResult() const
{
	return Result;
}

VOID BLRetAuth::SetResult(LOGIN_RESULT result)
{
	Result = result;
}

const PlayerID_t BLRetAuth::GetPlayerID() const
{
	return PlayerID;
}

VOID		BLRetAuth::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}

const GUID_t BLRetAuth::GetPlayerGUID() const
{
	return PlayerGUID;
}

VOID		BLRetAuth::SetPlayerGUID(GUID_t guid)
{
	PlayerGUID	 = guid;
}

