#include "stdafx.h" 
#include "CLAskRegPassPort.h"


BOOL CLAskRegPassPort::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	iStream.Read( (CHAR*)(szSuperPw), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	iStream.Read( (CHAR*)(szConfirmSuperPw), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	iStream.Read( (CHAR*)(szPwMd5), sizeof(CHAR)*MAX_MD5_SIZE);
	iStream.Read( (CHAR*)(szConfirmPwMd5), sizeof(CHAR)*MAX_MD5_SIZE);
	iStream.Read( (CHAR*)(szEmail), sizeof(CHAR)*MAX_EMAIL_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CLAskRegPassPort::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT);
	oStream.Write( (CHAR*)(szSuperPw), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	oStream.Write( (CHAR*)(szConfirmSuperPw), sizeof(CHAR)*MAX_SUPER_CODE_SIZE);
	oStream.Write( (CHAR*)(szPwMd5), sizeof(CHAR)*MAX_MD5_SIZE);
	oStream.Write( (CHAR*)(szConfirmPwMd5), sizeof(CHAR)*MAX_MD5_SIZE);
	oStream.Write( (CHAR*)(szEmail), sizeof(CHAR)*MAX_EMAIL_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT CLAskRegPassPort::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CLAskRegPassPortHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


const CHAR*	CLAskRegPassPort::GetAccount() const
{
	return	szAccount;
}

VOID CLAskRegPassPort::SetAccount(CHAR* pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = 0;
}

const CHAR* CLAskRegPassPort::GetSuperPw() const
{
	return szSuperPw;
}

VOID CLAskRegPassPort::SetSuperPw(CHAR* pSuperPw)
{
	Assert(pSuperPw);
	strncpy(szSuperPw,pSuperPw,MAX_SUPER_CODE_SIZE*sizeof(CHAR));
	szSuperPw[MAX_SUPER_CODE_SIZE] = 0;
}

const CHAR* CLAskRegPassPort::GetConfirmSuperPw() const
{
	return szConfirmSuperPw;
}

VOID CLAskRegPassPort::SetConfirmSuperPw(CHAR* pConfirmSuperPw)
{
	Assert(pConfirmSuperPw);
	strncpy(szConfirmSuperPw,pConfirmSuperPw,MAX_SUPER_CODE_SIZE*sizeof(CHAR));
	szConfirmSuperPw[MAX_SUPER_CODE_SIZE] = 0;
}

const CHAR* CLAskRegPassPort::GetPwMd5() const
{
	return szPwMd5;
}

VOID CLAskRegPassPort::SetPwMd5(CHAR* pPwMd5)
{
	Assert(pPwMd5);
	strncpy(szPwMd5,pPwMd5,MAX_MD5_SIZE*sizeof(CHAR));
	szPwMd5[MAX_MD5_SIZE] = 0;
}

const CHAR* CLAskRegPassPort::GetConfirmPwMd5() const
{
	return szConfirmPwMd5;
}

VOID CLAskRegPassPort::SetConfirmPwMd5(CHAR* pConfirmPwMd5)
{
	Assert(pConfirmPwMd5);
	strncpy(szConfirmPwMd5,pConfirmPwMd5,MAX_MD5_SIZE*sizeof(CHAR));
	szConfirmPwMd5[MAX_MD5_SIZE] = 0;
}

const CHAR* CLAskRegPassPort::GetEmail() const
{
	return szEmail;
}

VOID CLAskRegPassPort::SetEmail(CHAR* pEmail)
{
	Assert(pEmail);
	strncpy(szEmail,pEmail,MAX_EMAIL_SIZE*sizeof(CHAR));
	szEmail[MAX_EMAIL_SIZE] = 0;
}
