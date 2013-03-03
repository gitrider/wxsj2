#include "stdafx.h" 
#include "LBAskAuth.h"

BOOL LBAskAuth::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(szIP), sizeof(CHAR)*MAX_BILLING_IP_SIZE);
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

BOOL LBAskAuth::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	oStream.Write((CHAR*)(&szIP),sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		oStream.Write( (CHAR*)(mAllMiBaoKey[i]),sizeof(CHAR)*MIBAOUNIT_NAME_LENGTH);
	}
	for(INT i = 0; i < MIBAOUNIT_NUMBER; ++i)
	{
		oStream.Write( (CHAR*)(mAllMiBaoValue[i]),sizeof(CHAR)*MIBAOUNIT_VALUE_LENGTH);
	}
	oStream.Write( (CHAR*)(mMacAddr), sizeof(CHAR)*MAX_MD5_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBAskAuth::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskAuthHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	LBAskAuth::GetAccount()	const
{
	return szAccount;
}
VOID			LBAskAuth::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT);
	szAccount[MAX_ACCOUNT] = 0 ;
}

const	CHAR*	LBAskAuth::GetPassWord()	const
{
	return szPassWord;
}
VOID			LBAskAuth::SetPassWord(const CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_MD5_SIZE);
	szPassWord[MAX_MD5_SIZE] = 0 ;
}
const PlayerID_t LBAskAuth::GetPlayerID() const
{
	return PlayerID;
}

VOID		LBAskAuth::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}

VOID	LBAskAuth::SetIPAddr(const CHAR* pIP)
{
	Assert(pIP);
	strncpy(szIP,pIP,MAX_BILLING_IP_SIZE);
	szIP[MAX_BILLING_IP_SIZE] = 0;
}

const CHAR* LBAskAuth::GetIPAddr() const
{
	return szIP;
}

const	CHAR*	LBAskAuth::GetMacAddr()	const
{
	return mMacAddr;
}
VOID			LBAskAuth::SetMacAddr(CHAR* pMacMD5)
{
	Assert(pMacMD5);
	strncpy(mMacAddr,pMacMD5,MAX_MD5_SIZE*sizeof(CHAR));
	mMacAddr[MAX_MD5_SIZE] = 0;
}
