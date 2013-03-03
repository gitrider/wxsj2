#include "stdafx.h" 
#include "LBAskRegLicense.h"

BOOL LBAskRegLicense::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	iStream.Read( (CHAR*)(&UserAnswer), sizeof(BYTE));
	iStream.Read( (CHAR*)(&PlayerID), sizeof(PlayerID_t));
	iStream.Read( (CHAR*)(szIP), sizeof(CHAR)*MAX_BILLING_IP_SIZE);

	return TRUE ;
	
	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL LBAskRegLicense::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(&UserAnswer), sizeof(BYTE));
	oStream.Write((CHAR*)(&PlayerID),sizeof(PlayerID_t));
	oStream.Write((CHAR*)(&szIP),sizeof(CHAR)*MAX_BILLING_IP_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT LBAskRegLicense::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return LBAskRegLicenseHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}

const	CHAR*	LBAskRegLicense::GetAccount()	const
{
	return szAccount;
}
VOID			LBAskRegLicense::SetAccount(const CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = 0 ;
}

const PlayerID_t LBAskRegLicense::GetPlayerID() const
{
	return PlayerID;
}

VOID		LBAskRegLicense::SetPlayerID(PlayerID_t pID)
{
	PlayerID = pID;
}

const BYTE LBAskRegLicense::GetUserAnswer() const
{
	return UserAnswer;
}

VOID LBAskRegLicense::SetUserAnswer(BYTE rt)
{
	UserAnswer = rt;
}

VOID	LBAskRegLicense::SetIPAddr(const CHAR* pIP)
{
	Assert(pIP);
	strncpy(szIP,pIP,MAX_BILLING_IP_SIZE);
	szIP[MAX_BILLING_IP_SIZE] = 0;
}

const CHAR* LBAskRegLicense::GetIPAddr() const
{
	return szIP;
}