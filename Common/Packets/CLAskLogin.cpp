#include "stdafx.h" 
#include "CLAskLogin.h"


BOOL CLAskLogin::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

		iStream.Read( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
		iStream.Read( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
		iStream.Read( (CHAR*)(&uVersion), sizeof(UINT));
		iStream.Read((CHAR*)mAllMiBaoValue,sizeof(mAllMiBaoValue));
		iStream.Read( (CHAR*)(mMacAddr), sizeof(CHAR)*MAX_MD5_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL CLAskLogin::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(szAccount), sizeof(CHAR)*MAX_ACCOUNT) ;
	oStream.Write( (CHAR*)(szPassWord), sizeof(CHAR)*MAX_MD5_SIZE);
	oStream.Write( (CHAR*)(&uVersion), sizeof(UINT));
	oStream.Write((CHAR*)mAllMiBaoValue,sizeof(mAllMiBaoValue));
	oStream.Write( (CHAR*)(mMacAddr), sizeof(CHAR)*MAX_MD5_SIZE);
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CLAskLogin::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return CLAskLoginHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}

const	CHAR*	CLAskLogin::GetAccount()	const
{
	return szAccount;
}
VOID			CLAskLogin::SetAccount(CHAR*	pAccount)
{
	Assert(pAccount);
	strncpy(szAccount,pAccount,MAX_ACCOUNT*sizeof(CHAR));
	szAccount[MAX_ACCOUNT] = 0 ;
}

const	CHAR*	CLAskLogin::GetPassWord()	const
{
	return szPassWord;
}
VOID			CLAskLogin::SetPassWord(CHAR*	pPassWord)
{
	Assert(pPassWord);
	strncpy(szPassWord,pPassWord,MAX_MD5_SIZE*sizeof(CHAR));
}

UINT			CLAskLogin::GetVersion() const
{
	return	uVersion;
}

VOID			CLAskLogin::SetVersion(UINT version)
{
	uVersion = version;
}

INT				CLAskLogin::GetPassWordLength()
{
	for(INT i = 0 ;i<MAX_MD5_SIZE+1;i++)
	{
		if(szPassWord[i]=='\0')
		{
			return i;
		}
	}
	return 0;
}

CHAR*	CLAskLogin::GetMacAddr()
{
	return mMacAddr;
}
VOID			CLAskLogin::SetMacAddr(CHAR* pMacMD5)
{
	Assert(pMacMD5);
	strncpy(mMacAddr,pMacMD5,MAX_MD5_SIZE*sizeof(CHAR));
}
