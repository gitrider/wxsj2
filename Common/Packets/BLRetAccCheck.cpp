#include "stdafx.h" 
#include "BLRetAccCheck.h"



BOOL BLRetAccCheck::Read( SocketBillingInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read((CHAR*)&mAccLength,sizeof(BYTE));
		if(mAccLength>MAX_BILLACCOUNT) mAccLength = MAX_BILLACCOUNT;
		iStream.Read(mAccount,mAccLength*sizeof(CHAR));
		iStream.Read((CHAR*)&mAccResult,sizeof(BYTE));
		
		if(mAccResult==0x01 || mAccResult==0xF1)
		{
			iStream.Read((CHAR*)&mCardPoint,sizeof(UINT));
			mCardPoint= ntohl(mCardPoint);
			iStream.Read((CHAR*)&mCardDay,sizeof(USHORT));
			mCardDay	= ntohs(mCardDay);
			iStream.Read((CHAR*)&mIsFatigue,sizeof(CHAR));
			iStream.Read((CHAR*)&mAccTotalOnlineSecond,sizeof(UINT));
			mAccTotalOnlineSecond = ntohl(mAccTotalOnlineSecond);
			iStream.Read((CHAR*)&mIsPhoneBind,sizeof(CHAR));
			iStream.Read((CHAR*)&mIsIPBind,sizeof(CHAR));
			iStream.Read((CHAR*)&mIsMiBaoBind,sizeof(CHAR));
			iStream.Read((CHAR*)&mIsMacBind,sizeof(CHAR));
			iStream.Read((CHAR*)&mIsRealNameBind,sizeof(CHAR));
			iStream.Read((CHAR*)&mIsInputNameBind,sizeof(CHAR));
		}
		else if(mAccResult==0x05)
		{
			iStream.Read((CHAR*)&mWorldID,sizeof(USHORT));
			mWorldID = ntohs(mWorldID);
			iStream.Read((CHAR*)&mIPLength,sizeof(BYTE));
			
			if(mIPLength>MAX_BILLING_IP_SIZE)	
				mIPLength = MAX_BILLING_IP_SIZE;
			iStream.Read((CHAR*)&mIP,sizeof(CHAR)*mIPLength);
			iStream.Read((CHAR*)&mServerNameLength,sizeof(BYTE));
			if(mServerNameLength>MAX_SERVER_NAME)
				mServerNameLength = MAX_SERVER_NAME;
			iStream.Read((CHAR*)&mSName,sizeof(CHAR)*mServerNameLength);

		}
			

		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BLRetAccCheck::Write( SocketBillingOutputStream& oStream )	const
{
	__ENTER_FUNCTION

	oStream.Write((CHAR*)&mAccLength,sizeof(BYTE));
	oStream.Write(mAccount,mAccLength*sizeof(CHAR));
	oStream.Write((CHAR*)&mAccResult,sizeof(BYTE));

	if(mAccResult==0x01 || mAccResult==0xF1)
	{
		UINT CardPoint	= htonl(mCardPoint);
		oStream.Write((CHAR*)&CardPoint,sizeof(UINT));
		USHORT CardDay	= htons(mCardDay);
		oStream.Write((CHAR*)&CardDay,sizeof(USHORT));
		oStream.Write((CHAR*)&mIsFatigue,sizeof(CHAR));
		UINT AccTotalOnlineSecond = htonl(mAccTotalOnlineSecond);
		oStream.Write((CHAR*)&AccTotalOnlineSecond,sizeof(UINT));
		oStream.Write((CHAR*)&mIsPhoneBind,sizeof(CHAR));
		oStream.Write((CHAR*)&mIsIPBind,sizeof(CHAR));
		oStream.Write((CHAR*)&mIsMiBaoBind,sizeof(CHAR));
		oStream.Write((CHAR*)&mIsMacBind,sizeof(CHAR));
		oStream.Write((CHAR*)&mIsRealNameBind,sizeof(CHAR));
		oStream.Write((CHAR*)&mIsInputNameBind,sizeof(CHAR));
	}
	else if(mAccResult==0x05)
	{
		USHORT WorldID = htons(mWorldID);
		oStream.Write((CHAR*)&WorldID,sizeof(USHORT));
		oStream.Write((CHAR*)&mIPLength,sizeof(BYTE));
		oStream.Write((CHAR*)&mIP,sizeof(CHAR)*mIPLength);
		oStream.Write((CHAR*)&mServerNameLength,sizeof(BYTE));
		oStream.Write((CHAR*)&mSName,sizeof(CHAR)*mServerNameLength);

	}
		return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BLRetAccCheck::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BLRetAccCheckHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	BLRetAccCheck::GetAccount()	const
{
	return mAccount;
}
VOID			BLRetAccCheck::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(mAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	mAccount[MAX_ACCOUNT] = '\0' ;
	mAccLength = (BYTE)strlen(pAccount);
}

BYTE	BLRetAccCheck::GetResult() const
{
	return mAccResult;
}

VOID BLRetAccCheck::SetResult(BYTE result)
{
	mAccResult = result;
}
