#include "stdafx.h" 
#include "LBAskRegAuth.h"

BOOL LBAskRegAuth::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	iStream.Read( (CHAR*)(szSuperPassWord), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
	iStream.Read( (CHAR*)(szIP), sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	iStream.Read( (CHAR*)(szEmail), sizeof(CHAR)*MAX_EMAIL_SIZE);
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));

	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBAskRegAuth::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szSuperPassWord), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
	oStream.Write( (CHAR*)(szIP), sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	oStream.Write( (CHAR*)(szEmail), sizeof(CHAR)*MAX_EMAIL_SIZE);
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBAskRegAuth::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskRegAuthHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	LBAskRegAuth::GetAccount()	const
{
	return szAccount;
}
VOID			LBAskRegAuth::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = 0 ;
}

const	CHAR*	LBAskRegAuth::GetPassWord()	const
{
	return szPassWord;
}
VOID			LBAskRegAuth::SetPassWord(const CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_MD5_SIZE*sizeof(CHAR));
	szPassWord[MAX_MD5_SIZE] = 0 ;
}
const PlayerID_t LBAskRegAuth::GetPlayerID() const
{
	return PlayerID;
}

VOID		LBAskRegAuth::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}

const CHAR* LBAskRegAuth::GetSuperPassWord() const
{
	return szSuperPassWord;
}

VOID LBAskRegAuth::SetSuperPassWord(const CHAR* pSuperPassWord)
{
	Assert(pSuperPassWord);
	strncpy(szSuperPassWord,pSuperPassWord,MAX_SUPER_CODE_SIZE*sizeof(CHAR));
	szSuperPassWord[MAX_SUPER_CODE_SIZE] = 0 ;
}

const CHAR* LBAskRegAuth::GetIP() const
{
	return szIP;
}

VOID LBAskRegAuth::SetIP(const CHAR* pIP)
{
	Assert(pIP);
	strncpy(szIP,pIP,MAX_BILLING_IP_SIZE*sizeof(CHAR));
	szIP[MAX_BILLING_IP_SIZE] = 0 ;
}

const CHAR* LBAskRegAuth::GetEmail() const
{
	return szEmail;
}

VOID LBAskRegAuth::SetEmail(const CHAR* pEmail)
{
	Assert(pEmail);
	strncpy(szEmail,pEmail,MAX_EMAIL_SIZE*sizeof(CHAR));
	szEmail[MAX_EMAIL_SIZE] = 0 ;
}
